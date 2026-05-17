/**
 * @file src/SceneSystem/SceneLoader/SceneLoader.hpp
 * @brief Loads scene graphs from parsed configuration nodes.
 *
 * Declares `SceneLoader`, which maps configuration nodes to plugin initializers
 * to build a `RayTracer::Scene`. This module decouples parsing from object
 * construction and uses @ref PluginManager for plugin-provided factories.
 */
#pragma once
#include "../PluginManager/PluginManager.hpp"
#include "../ConfigParser/Parser.hpp"
#include "../Scene/Scene.hpp"
#include "../../Exceptions/RaytracerException/RaytracerException.hpp"

namespace RayTracer
{
    class SceneLoader
    {
    private:
        PluginManager& _pluginManager; ///< Manages plugins for the scene loader
        Parser _parser;               ///< Parses configuration files
    public:
        SceneLoader(PluginManager &manager, const std::string &filename);
        ~SceneLoader() = default;

        /// Load a scene from a configuration file.
        /// @param filename Path to the configuration file
        /// @return An unique pointer to the loaded scene object
        std::unique_ptr<Scene> loadScene(void);
    };
}
