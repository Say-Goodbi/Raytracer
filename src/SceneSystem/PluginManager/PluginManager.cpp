#include "PluginManager.hpp"
#include <dirent.h>
#include <iostream>

PluginManager::PluginManager(std::string pluginDirectory) : _pluginDirectory(pluginDirectory)
{
    loadPlugins();
}

void PluginManager::loadPlugins()
{
    _plugins.clear();
    _plugins["CorePlugin"] = CorePlugin().getInitializers();
    struct dirent* entry = nullptr;
    DIR* dir = opendir(_pluginDirectory.c_str());
    if (!dir)
        throw Raytracer::Exception("Failed to open plugin directory: " + _pluginDirectory);
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename.size() > 3 && filename.substr(filename.size() - 3) == ".so") {
            std::string path = _pluginDirectory + "/" + filename;
            loadPlugin(path);
        }
    }
    closedir(dir);
}

bool PluginManager::loadPlugin(const std::string& path)
{
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load plugin: " << path << " - " << dlerror() << std::endl;
        return false;
    }
    IPlugin* (*getFunc)() = reinterpret_cast<IPlugin* (*)()>(dlsym(handle, "getPlugin"));
    if (!getFunc) {
        std::cerr << "Failed to find getPlugin function in: " << path << " - " << dlerror() << std::endl;
        dlclose(handle);
        return false;
    }
    IPlugin* pluginInstance = getFunc();
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
    std::map<std::string, std::function<Component(Setting)>> initializers = pluginInstance->getInitializers();
    delete pluginInstance;
    overrideInitializers(initializers);
    _plugins[pluginName] = initializers;
    return true;
}

void PluginManager::overrideInitializers(const std::map<std::string, std::function<Component(Setting)>>& newInitializers)
{
    for (const auto& pair : newInitializers) {
        const std::string& name = pair.first;
        for (auto& pluginPair : _plugins) {
            auto& initializers = std::get<1>(pluginPair.second);
            if (initializers.find(name) != initializers.end()) {
                initializers.erase(name); // Remove existing initializer with the same name
                std::cout << "Overriding initializer: " << name << " from plugin: " << pluginPair.first << std::endl;
            }
        }
    }
}

function<Component(Setting)> PluginManager::getInitializer(const std::string& name)
{
    for (const auto& pluginPair : _plugins) {
        const auto& initializers = std::get<1>(pluginPair.second);
        if (initializers.find(name) != initializers.end()) {
            return initializers.at(name);
        }
    }
    return nullptr; // Return nullptr if initializer not found
}