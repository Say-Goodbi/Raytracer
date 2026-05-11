#include "PluginManager.hpp"
#include <dirent.h>
#include <iostream>

RayTracer::PluginManager::PluginManager(std::string pluginDirectory) : _pluginDirectory(pluginDirectory)
{
    loadPlugins();
}

void RayTracer::PluginManager::loadPlugins()
{
    _plugins.clear();
    _plugins["CorePlugin"] = CorePlugin().getInitializers();
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
    closedir(dir);
}

bool RayTracer::PluginManager::loadPlugin(const std::string& path)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << path << " - " << dlerror() << std::endl;
        return false;
    }
    RayTracer::IPlugin* (*getFunc)() = reinterpret_cast<RayTracer::IPlugin* (*)()>(dlsym(handle, "getPlugin"));
    if (!getFunc) {
        std::cerr << "Failed to find getPlugin function in: " << path << " - " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }
    RayTracer::IPlugin* pluginInstance = getFunc();
    if (!pluginInstance) {
        std::cerr << "Failed to create plugin instance from: " << path << std::endl;
        dlclose(handle);
        return false;
    }
    std::string pluginName = pluginInstance->getName();
    if (_plugins.find(pluginName) != _plugins.end()) {
        std::cerr << "Plugin with name " << pluginName << " already loaded. Skipping: " << path << std::endl;
        delete pluginInstance;
        dlclose(handle);
        return false;
    }
    std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>> initializers = pluginInstance->getInitializers();
    delete pluginInstance;
    overrideInitializers(initializers);
    _plugins[pluginName] = initializers;
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
    return nullptr;
}
