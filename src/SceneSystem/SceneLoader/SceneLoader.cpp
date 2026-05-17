/**
 * @file src/SceneSystem/SceneLoader/SceneLoader.cpp
 * @brief Scene parsing and construction from configuration nodes.
 *
 * Implements the `SceneLoader` which walks parsed config nodes and uses
 * `PluginManager` initializers to create renderers, primitives and lights,
 * assembling a `RayTracer::Scene` instance. See @ref SceneSystem::ConfigParser
 * for parsing details.
 */
#include "SceneLoader.hpp"

namespace RayTracer {
    std::map<std::string, std::function<void(NodePtr&, PluginManager&, std::unique_ptr<Scene>&)>> _componentCreators = {
        {"camera", [](NodePtr &obj, PluginManager &pluginManager, std::unique_ptr<Scene> &scene) { scene->updateCamera(obj, pluginManager); }},
        {"primitives", [](NodePtr &obj, PluginManager &pluginManager, std::unique_ptr<Scene> &scene) { scene->addPrimitives(obj, pluginManager); }},
        {"lights", [](NodePtr &obj, PluginManager &pluginManager, std::unique_ptr<Scene> &scene) { scene->addLights(obj, pluginManager); }}
    };

    SceneLoader::SceneLoader(PluginManager &manager, const std::string &filename) : _pluginManager(manager), _parser(filename) {}

    std::unique_ptr<Scene> SceneLoader::loadScene(void)
    {
        std::unique_ptr<Scene> scene = std::make_unique<Scene>();

        if (!this->_parser._node)
            throw RayTracer::ParsingException("Failed to parse scene configuration file.");
        for (auto &[componentKey, creator] : _componentCreators) {
            auto it = std::get<Object>(_parser._node->value).find(componentKey);
            if (it == std::get<Object>(_parser._node->value).end())
                throw RayTracer::ParsingException("Missing required component in scene configuration: " + componentKey);
            creator(it->second, _pluginManager, scene);
        }
        return scene;
    }
}