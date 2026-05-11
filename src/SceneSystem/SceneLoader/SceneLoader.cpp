#include "SceneLoader.hpp"

namespace RayTracer {
    std::map<std::string, std::function<void(NodePtr&, PluginManager&, Scene&)>> _componentCreators = {
        {"camera", [](NodePtr &obj, PluginManager &pluginManager, Scene &scene) { scene.updateCamera(obj, pluginManager); }},
        {"primitives", [](NodePtr &obj, PluginManager &pluginManager, Scene &scene) { scene.addPrimitives(obj, pluginManager); }},
        {"lights", [](NodePtr &obj, PluginManager &pluginManager, Scene &scene) { scene.addLights(obj, pluginManager); }}
    };

    SceneLoader::SceneLoader(const std::string &filename) : _pluginManager("plugins"), _parser(filename) {}

    // Scene SceneLoader::createCamera(Object &cameraSettings)
    // {
    //     auto initializer = _pluginManager.getInitializer("camera");
    //     if (!initializer)
    //         throw RayTracer::Exception("Camera initializer not found in plugins.");
    //     return std::get<ARenderer>(initializer(cameraSettings));
    // }

    // std::unique_ptr<APrimitive> SceneLoader::createPrimitive(Object &primitiveSettings)
    // {
    //     const auto &primitives = std::get<Object>(_parser._node->value).at("primitives");

    //     if (std::get<Object>(primitives->value).find("spheres") != std::get<Object>(primitives->value).end())
    //     { // TO DO
    //         const Object &spheres = std::get<Object>(primitives->value).at("spheres");
    //         for (const Object &sphereNode : std::get<Object>(spheres->value))
    //             for (const auto &[key, nodePtr] : sphereNode)
    //             {
    //                 Sphere sphere = new Sphere();
    //                 _scene.addPrimitive(sphere);
    //             }
    //     }
    //     if (std::get<Object>(primitives->value).find("planes") != std::get<Object>(primitives->value).end())
    //     {
    //         const Object &planes = std::get<Object>(primitives->value).at("planes");
    //         Geometry::Point3D point = {0.0, 0.0, 0.0};
    //         Geometry::Vector3D normal = {0.0, 0.0, 0.0};
    //         Color color = {0.0, 0.0, 0.0};
    //         for (const Object &planesNode : std::get<Object>(planes->value))
    //         {
    //             for (const auto &[key, nodePtr] : planesNode)
    //             {
    //                 if (key == "point")
    //                     point = {
    //                         .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
    //                         .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
    //                         .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
    //                 else if (key == "normal")
    //                     normal = {
    //                         .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
    //                         .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
    //                         .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
    //                 else if (key == "color")
    //                     color = {
    //                         .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
    //                         .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
    //                         .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
    //             }
    //             Plane plane = new Plane(point, normal, color);
    //             _scene.addPrimitive(plane);
    //         }
    //     }
    //     return primitive;
    // }

    // std::unique_ptr<ILight> SceneLoader::createLight(Object &lightSettings)
    // {
    //     const Object &lights = std::get<Object>(_parser._node->value).at("lights");

    //     if (std::get<Object>(lights->value).find("point") != std::get<Object>(lights->value).end())
    //     {
    //         const Object &point = std::get<Object>(lights->value).at("point");
    //         Geometry::Point3D _position = {0.0, 0.0, 0.0};
    //         double _intensity = 0.0;
    //         Color _color = {0.0, 0.0, 0.0};
    //         for (const Object &sphereNode : std::get<Object>(point->value))
    //         {
    //             for (const auto &[key, nodePtr] : sphereNode)
    //             {
    //                 if (key == "position")
    //                     _position = {
    //                         .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
    //                         .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
    //                         .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
    //                 else if (key == "intensity")
    //                     _intensity = std::get<double>(std::get<ScalarValue>(nodePtr->value));
    //                 else if (key == "color")
    //                     _color = {
    //                         .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
    //                         .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
    //                         .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
    //             }
    //             PointLight pointLight = new PointLight(_position, _intensity, _color);
    //             _scene.addLight(pointLight);
    //         }
    //     }
    //     if (std::get<Object>(lights->value).find("directional") != std::get<Object>(lights->value).end())
    //     {
    //         const Object &directional = std::get<Object>(lights->value).at("directional");
    //         Geometry::Vector3D _direction = {0.0, 0.0, 0.0};
    //         double _intensity = 0.0;
    //         Color _color = {0.0, 0.0, 0.0};
    //         for (const Object &sphereNode : std::get<Object>(directional->value))
    //         {
    //             DirectionalLight directionalLight = new DirectionalLight();
    //             for (const auto &[key, nodePtr] : sphereNode)
    //             {
    //                 if (key == "direction")
    //                     _direction = {
    //                         .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
    //                         .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
    //                         .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
    //                 else if (key == "intensity")
    //                     _intensity = std::get<double>(std::get<ScalarValue>(nodePtr->value));
    //                 else if (key == "color")
    //                     _color = {
    //                         .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
    //                         .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
    //                         .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
    //             }
    //             _scene.addLight(directionalLight);
    //         }
    //     }
    //     if (std::get<Object>(lights->value).find("ambient") != std::get<Object>(lights->value).end())
    //     {
    //         const double &ambient = std::get<double>(std::get<ScalarValue>(std::get<Object>(lights->value).at("ambient")->value));
    //         Color _color = {0.0, 0.0, 0.0};

    //         AmbientLight ambientLight = new AmbientLight();
    //         const auto &color = std::get<Object>(lights->value).at("color");
    //         _color = {
    //             .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("r")->value)),
    //             .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("g")->value)),
    //             .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("b")->value))};
    //         _scene.addLight(ambientLight);
    //     }
    //     return primitive;
    // }

    Scene *SceneLoader::loadScene(void)
    {
        Scene *scene = new Scene();

        if (!this->_parser._node)
            throw RayTracer::Exception("Failed to parse scene configuration file.");
        for (auto &[key, nodePtr] : std::get<Object>(_parser._node->value))
            for (auto &[componentKey, creator] : _componentCreators)
                if (key == componentKey && nodePtr->value.index() == 1){
                    creator(std::get<Object>(nodePtr->value).at(componentKey), _pluginManager, *scene);
                    break;
                }
        return scene;
    }
}