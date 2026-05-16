#include "PluginManager.hpp"

RayTracer::PluginManager::PluginManager(std::string pluginDirectory) : _pluginDirectory(pluginDirectory)
{
    loadPlugins();
}

RayTracer::PluginManager::~PluginManager()
{
    for (void* h : _handles) {
        //if (h)
          //  dlclose(h);
    }
}

void RayTracer::PluginManager::loadPlugins()
{
    _plugins.clear();
    struct dirent* entry = nullptr;
    DIR* dir = opendir(_pluginDirectory.c_str());
    if (!dir)
        throw RayTracer::Exception("Failed to open plugin directory: " + _pluginDirectory);
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename.size() > 3 && filename.substr(filename.size() - 3) == ".so") {
            std::string path = _pluginDirectory + "/" + filename;
            loadPlugin(path);
        }
    }
    if (_plugins.find("CorePlugin") == _plugins.end())
        std::cerr << "Warning: CorePlugin not found. The application may not function correctly without it." << std::endl;
    closedir(dir);
}

bool RayTracer::PluginManager::loadPlugin(const std::string& path)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << path << " - " << dlerror() << std::endl;
        return false;
    }
    using GetNameFn = std::string (*)();
    using InitializersMap = std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>>;
    using GetInitializersFn = InitializersMap (*)();

    GetNameFn getName = reinterpret_cast<GetNameFn>(dlsym(handle, "getName"));
    GetInitializersFn getInitializers = reinterpret_cast<GetInitializersFn>(dlsym(handle, "getInitializers"));

    if (getName == nullptr || getInitializers == nullptr) {
        std::cerr << "Failed to find plugin functions in: " << path << " - " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }
    std::string pluginName = getName();
    if (_plugins.find(pluginName) != _plugins.end()) {
        std::cerr << "Plugin with name " << pluginName << " already loaded. Skipping: " << path << std::endl;
        dlclose(handle);
        return false;
    }
    InitializersMap initializers = getInitializers();
    overrideInitializers(initializers);
    _plugins[pluginName] = initializers;
    _handles.push_back(handle);
    return true;
}

void RayTracer::PluginManager::overrideInitializers(const std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>>& newInitializers)
{
    for (const auto& pair : newInitializers) {
        const std::string& name = pair.first;
        for (auto& pluginPair : _plugins) {
            auto& initializers = pluginPair.second;
            if (initializers.find(name) != initializers.end()) {
                initializers.erase(name);
                std::cout << "Overriding initializer: " << name << " from plugin: " << pluginPair.first << std::endl;
            }
        }
    }
}

std::function<RayTracer::Component(RayTracer::NodePtr)> RayTracer::PluginManager::getInitializer(const std::string& name)
{
    for (const auto& pluginPair : _plugins) {
        const auto& initializers = pluginPair.second;
        if (initializers.find(name) != initializers.end()) {
            return initializers.at(name);
        }
    }
    return std::function<RayTracer::Component(RayTracer::NodePtr)>();
}
