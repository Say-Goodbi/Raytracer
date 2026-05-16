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
            throw RayTracer::Exception("Failed to parse scene configuration file.");
        for (auto &[key, nodePtr] : std::get<Object>(_parser._node->value))
            for (auto &[componentKey, creator] : _componentCreators) {
                if (key != componentKey)
                    continue;
                creator(nodePtr, _pluginManager, scene);
                break;
            }
        return scene;
    }
}