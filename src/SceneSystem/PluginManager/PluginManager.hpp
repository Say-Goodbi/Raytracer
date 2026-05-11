/**
 * @file PluginManager.hpp
 * @brief Plugin manager for loading and managing plugins
 * 
 * @details This file defines the PluginManager class, which is responsible for loading and managing plugins in the ray tracer application. 
 * The PluginManager allows for dynamic loading of plugins at runtime, enabling users to extend the functionality of the ray tracer without modifying the core codebase.
 */

#pragma once
#include <dlfcn.h>
// Only AInterface, APrimitive, ARenderer, ILight and IMaterial are relevant for plugins.
#include "../../Objects/IPlugin.hpp"

namespace RayTracer
{
    class PluginManager
    {
        private:
            std::string _pluginDirectory; ///< Directory where plugins are located
            std::map<std::string, std::map<std::string, std::function<Component(NodePtr)>>> _plugins; ///< Map of plugin names to their keyword-initializer pairs

            /// Load a plugin from a shared library file.
            /// @param path Filesystem path to the shared library (.so)
            /// @return True if the plugin was loaded successfully, false otherwise
            bool loadPlugin(const std::string& path);
            void overrideInitializers(const std::map<std::string, std::function<Component(NodePtr)>>& newInitializers);

        public:
            PluginManager(std::string pluginDirectory); ///< Constructor that loads plugins from the default directory;
            ~PluginManager() = default;

            /// Loads all plugins within the directory.
            void loadPlugins();

            /// Get an initializer function pointer from a loaded plugin.
            /// @param name Identifier of the initializer to retrieve
            /// @return Pointer to the requested function, or nullptr if not found
            std::function<Component(NodePtr)> getInitializer(const std::string& name);
    };
} // namespace RayTracer