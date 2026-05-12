#include "SceneSystem/Scene/Scene.hpp"
#include "SceneSystem/SceneLoader/SceneLoader.hpp"
#include "Exceptions/RaytracerException/RaytracerException.hpp"
#include <iostream>

int main(void)
{
    try
    {
        RayTracer::SceneLoader loader("./config/example.cfg");
        std::unique_ptr<RayTracer::Scene> scene(loader.loadScene());
        if (!scene)
            throw RayTracer::Exception("Failed to load scene from configuration file.");
        // Render the scene here using the loaded scene object
    }
    catch (const RayTracer::Exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
