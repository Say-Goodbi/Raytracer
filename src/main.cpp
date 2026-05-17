/**
 * @file src/main.cpp
 * @brief Entry point and CLI for the raytracer application.
 *
 * This binary parses command-line arguments, loads a scene configuration via
 * the `SceneLoader` and dispatches rendering through the plugin-provided
 * interface (e.g., @ref SceneWriter). It wires together @ref PluginManager,
 * @ref SceneLoader and the runtime interfaces used for output.
 */
#include "SceneSystem/Scene/Scene.hpp"
#include "SceneSystem/SceneLoader/SceneLoader.hpp"
#include "Exceptions/RaytracerException/RaytracerException.hpp"
#include "Objects/Plugin.hpp"
#include <iostream>

static std::map<std::string, std::string> parse_parameters(int ac, char *av[])
{
    std::map<std::string, std::string> parameters;
    for (int i = 2; i < ac; ++i) {
        std::string arg(av[i]);
        if (!arg.empty() && arg[0] == '-' && arg.size() > 1) {
            std::string key = arg.substr(1);
            if (i + 1 < ac) {
                parameters[key] = av[++i];
            } else {
                parameters[key] = ""; // Flag without value
            }
        }
    }
    return parameters;
}

int main(int ac, char *av[])
{
    try {
        if (ac < 2) {
            std::cerr << "Usage: " << av[0] << " <scene_config_file> [-i <interface> [flags]]" << std::endl;
            return EXIT_FAILURE;
        }
        RayTracer::PluginManager pluginManager("plugins");
        RayTracer::SceneLoader loader(pluginManager, av[1]);
        std::unique_ptr<RayTracer::Scene> scene(loader.loadScene());
        if (!scene)
            throw RayTracer::Exception("Failed to load scene from configuration file.");
        // Render the scene here using the loaded scene object
        std::map<std::string, std::string> parameters = parse_parameters(ac, av);
        std::function<RayTracer::Component(RayTracer::NodePtr)> interfaceInitializer = pluginManager.getInitializer(
            parameters.find("i") != parameters.end() ? parameters.find("i")->second : "SceneWriter"
        );
        if (!interfaceInitializer)
            throw RayTracer::Exception("Interface initializer not found in plugins.");

        RayTracer::Component interfaceComponent = interfaceInitializer(nullptr);
        if (!std::holds_alternative<std::shared_ptr<RayTracer::AInterface>>(interfaceComponent))
            throw RayTracer::Exception("Requested initializer did not create an interface component.");

        std::shared_ptr<RayTracer::AInterface> interface = std::get<std::shared_ptr<RayTracer::AInterface>>(interfaceComponent);
        if (!interface)
            throw RayTracer::Exception("Interface initializer returned a null interface.");

        interface->setRenderer(scene->getRenderer().get());
        interface->execute(*scene, parameters);
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
