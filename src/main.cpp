#include "SceneSystem/Scene/Scene.hpp"
#include "SceneSystem/SceneLoader/SceneLoader.hpp"
#include "Exceptions/RaytracerException/RaytracerException.hpp"
#include "Objects/Plugins/CorePlugin/Interfaces/SceneWriter/SceneWriter.hpp"
#include <iostream>

int main(int ac, char *av[])
{
    try {
        if (ac < 2) {
            std::cerr << "Usage: " << av[0] << " <scene_config_file>" << std::endl;
            return EXIT_FAILURE;
        }
        RayTracer::SceneLoader loader(av[1]);
        std::unique_ptr<RayTracer::Scene> scene(loader.loadScene());
        if (!scene)
            throw RayTracer::Exception("Failed to load scene from configuration file.");
        // Render the scene here using the loaded scene object
        RayTracer::SceneWriter writer(scene->getRenderer().get(), "output.ppm");
        writer.execute(*scene);
    }
    catch (const RayTracer::Exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
