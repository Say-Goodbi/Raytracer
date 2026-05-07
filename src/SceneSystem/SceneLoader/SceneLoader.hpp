#include "RaytracerException.hpp"
#include "PluginManager.hpp"
#include "Parser.hpp"
#include "Scene.hpp"

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