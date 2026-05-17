/**
 * @file src/SceneSystem/PluginManager/PluginManager.hpp
 * @brief Plugin loader interface for dynamic plugins.
 *
 * Declares the `PluginManager` which loads shared libraries and exposes
 * initializer factory functions for scene elements (renderers, primitives,
 * lights, interfaces). See @ref SceneLoader for how initializers are used.
 */
#pragma once
#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <vector>
// Only AInterface, APrimitive, ARenderer, ILight and IMaterial are relevant for plugins.
#include "../../Objects/Plugin.hpp"

namespace RayTracer
{
    class PluginManager
    {
        private:
            std::string _pluginDirectory; ///< Directory where plugins are located
            std::map<std::string, std::map<std::string, std::function<Component(NodePtr)>>> _plugins; ///< Map of plugin names to their keyword-initializer pairs
            std::vector<void*> _handles; ///< dlopen handles for loaded plugins

            void unloadPlugins();

            /// Load a plugin from a shared library file.
            /// @param path Filesystem path to the shared library (.so)
            /// @return True if the plugin was loaded successfully, false otherwise
            bool loadPlugin(const std::string& path);
            void overrideInitializers(const std::map<std::string, std::function<Component(NodePtr)>>& newInitializers);

        public:
            PluginManager(std::string pluginDirectory); ///< Constructor that loads plugins from the default directory;
            ~PluginManager();

            /// Loads all plugins within the directory.
            void loadPlugins();

            /// Get an initializer function pointer from a loaded plugin.
            /// @param name Identifier of the initializer to retrieve
            /// @return Pointer to the requested function, or nullptr if not found
            std::function<Component(NodePtr)> getInitializer(const std::string& name);
    };
} // namespace RayTracer