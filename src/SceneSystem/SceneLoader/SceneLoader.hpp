#pragma once
#include "../PluginManager/PluginManager.hpp"
#include "../ConfigParser/Parser.hpp"
#include "../Scene/Scene.hpp"
#include "../../Exceptions/RaytracerException/RaytracerException.hpp"

/**
 * @todo Always use pluginManager to create components (camera, primitives, lights) instead of hardcoding them in the scene loader. This will allow users to define custom components in their config files and have them created by the scene loader without modifying its code.
 * @todo Implement createPrimitive() and createLight() to handle all supported primitive and light types.
 * @todo Implement error handling for missing or invalid configuration settings (e.g., missing camera, invalid primitive parameters) and provide informative error messages to help users debug their config files.
 */
namespace RayTracer
{
    class SceneLoader
    {
    private:
        PluginManager _pluginManager; ///< Manages plugins for the scene loader
        Parser _parser;               ///< Parses configuration files
        RayTracer::Scene createCamera(RayTracer::Object &cameraSettings);
        std::unique_ptr<APrimitive> createPrimitive(RayTracer::Object &primitiveSettings);
        std::unique_ptr<ILight> createLight(RayTracer::Object &lightSettings);
    public:
        SceneLoader(const std::string &filename);
        ~SceneLoader() = default;

        /// Load a scene from a configuration file.
        /// @param filename Path to the configuration file
        /// @return A pointer to the loaded scene object
        RayTracer::Scene *loadScene(void);
    };
}