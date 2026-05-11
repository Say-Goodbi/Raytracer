#include "Scene.hpp"

namespace RayTracer
{
    Scene::Scene(ARenderer &camera) : _renderer(&camera, [](ARenderer *) {})
    {
    }

    Scene::Scene(void) : _renderer(nullptr) {}

    void Scene::updateCamera(RayTracer::NodePtr &camera, PluginManager &pluginManager)
    {
        auto initializer = pluginManager.getInitializer("camera");
        if (!initializer)
            throw RayTracer::Exception("Camera initializer not found in plugins.");
        this->_renderer = std::get<std::shared_ptr<ARenderer>>(initializer(std::move(camera)));
    }

    void Scene::addPrimitive(std::shared_ptr<APrimitive> primitive)
    {
        this->_primitives.push_back(primitive);
    }

    void Scene::addPrimitives(RayTracer::NodePtr &primitives, PluginManager &pluginManager)
    {
        for (auto &[key, nodePtr] : std::get<Object>(primitives->value))
        {
            Object &primitive = std::get<Object>(nodePtr->value);

            for (auto &[_, nodeValue] : primitive)
            {
                auto initializer = pluginManager.getInitializer(key);

                if (!initializer)
                    throw RayTracer::Exception("Primitive initializer not found in plugins for type: " + key);
                std::shared_ptr<APrimitive> primitive = std::get<std::shared_ptr<APrimitive>>(initializer(std::move(nodeValue)));
                this->addPrimitive(primitive);
            }
        }

        // if (std::get<Object>(primitives->value).find("spheres") != std::get<Object>(primitives->value).end())
        // { // TO DO
        //     const Object &spheres = std::get<Object>(primitives->value).at("spheres");
        //     for (const Object &sphereNode : std::get<Object>(spheres->value))
        //         for (const auto &[key, nodePtr] : sphereNode)
        //         {
        //             Sphere sphere = new Sphere();
        //             _scene.addPrimitive(sphere);
        //         }
        // }
        // if (std::get<Object>(primitives->value).find("planes") != std::get<Object>(primitives->value).end())
        // {
        //     const Object &planes = std::get<Object>(primitives->value).at("planes");
        //     Geometry::Point3D point = {0.0, 0.0, 0.0};
        //     Geometry::Vector3D normal = {0.0, 0.0, 0.0};
        //     Color color = {0.0, 0.0, 0.0};
        //     for (const Object &planesNode : std::get<Object>(planes->value))
        //     {
        //         for (const auto &[key, nodePtr] : planesNode)
        //         {
        //             if (key == "point")
        //                 point = {
        //                     .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
        //                     .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
        //                     .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
        //             else if (key == "normal")
        //                 normal = {
        //                     .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
        //                     .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
        //                     .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
        //             else if (key == "color")
        //                 color = {
        //                     .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
        //                     .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
        //                     .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
        //         }
        //         Plane plane = new Plane(point, normal, color);
        //         _scene.addPrimitive(plane);
        //     }
        // }
    }

    std::vector<std::shared_ptr<APrimitive>> &Scene::getPrimitives()
    {
        return this->_primitives;
    }

    void Scene::addLight(std::shared_ptr<ILight> light)
    {
        this->_lights.push_back(std::move(light));
    }

    void Scene::addLights(RayTracer::NodePtr &lights, PluginManager &pluginManager)
    {
        for (auto &[key, nodePtr] : std::get<Object>(lights->value))
        {
            Object &light = std::get<Object>(nodePtr->value);

            for (auto &[_, nodeValue] : light)
            {
                auto initializer = pluginManager.getInitializer(key);
    
                if (!initializer)
                    throw RayTracer::Exception("Light initializer not found in plugins for type: " + key);
                std::shared_ptr<ILight> light = std::get<std::shared_ptr<ILight>>(initializer(std::move(nodeValue)));
                this->addLight(light);
            }
        }

        // if (std::get<Object>(lights->value).find("point") != std::get<Object>(lights->value).end())
        // {
        //     const Object &point = std::get<Object>(lights->value).at("point");
        //     Geometry::Point3D _position = {0.0, 0.0, 0.0};
        //     double _intensity = 0.0;
        //     Color _color = {0.0, 0.0, 0.0};
        //     for (const Object &sphereNode : std::get<Object>(point->value))
        //     {
        //         for (const auto &[key, nodePtr] : sphereNode)
        //         {
        //             if (key == "position")
        //                 _position = {
        //                     .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
        //                     .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
        //                     .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
        //             else if (key == "intensity")
        //                 _intensity = std::get<double>(std::get<ScalarValue>(nodePtr->value));
        //             else if (key == "color")
        //                 _color = {
        //                     .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
        //                     .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
        //                     .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
        //         }
        //         PointLight pointLight = new PointLight(_position, _intensity, _color);
        //         _scene.addLight(pointLight);
        //     }
        // }
        // if (std::get<Object>(lights->value).find("directional") != std::get<Object>(lights->value).end())
        // {
        //     const Object &directional = std::get<Object>(lights->value).at("directional");
        //     Geometry::Vector3D _direction = {0.0, 0.0, 0.0};
        //     double _intensity = 0.0;
        //     Color _color = {0.0, 0.0, 0.0};
        //     for (const Object &sphereNode : std::get<Object>(directional->value))
        //     {
        //         DirectionalLight directionalLight = new DirectionalLight();
        //         for (const auto &[key, nodePtr] : sphereNode)
        //         {
        //             if (key == "direction")
        //                 _direction = {
        //                     .x = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("x")->value)),
        //                     .y = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("y")->value)),
        //                     .z = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("z")->value))};
        //             else if (key == "intensity")
        //                 _intensity = std::get<double>(std::get<ScalarValue>(nodePtr->value));
        //             else if (key == "color")
        //                 _color = {
        //                     .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("r")->value)),
        //                     .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("g")->value)),
        //                     .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(nodePtr->value).at("b")->value))};
        //         }
        //         _scene.addLight(directionalLight);
        //     }
        // }
        // if (std::get<Object>(lights->value).find("ambient") != std::get<Object>(lights->value).end())
        // {
        //     const double &ambient = std::get<double>(std::get<ScalarValue>(std::get<Object>(lights->value).at("ambient")->value));
        //     Color _color = {0.0, 0.0, 0.0};

        //     AmbientLight ambientLight = new AmbientLight();
        //     const auto &color = std::get<Object>(lights->value).at("color");
        //     _color = {
        //         .r = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("r")->value)),
        //         .g = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("g")->value)),
        //         .b = std::get<double>(std::get<ScalarValue>(std::get<Object>(color->value).at("b")->value))};
        //     _scene.addLight(ambientLight);
        // }
    }

    std::vector<std::shared_ptr<ILight>> &Scene::getLights()
    {
        return this->_lights;
    }

    const std::shared_ptr<ARenderer> &Scene::getRenderer() const
    {
        return this->_renderer;
    }

} // namespace RayTracer
