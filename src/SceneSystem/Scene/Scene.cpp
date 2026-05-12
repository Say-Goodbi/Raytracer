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
            VectorObject &primitive = std::get<VectorObject>(nodePtr->value);
            for (RayTracer::Object &obj : primitive)
            {
                auto initializer = pluginManager.getInitializer(key);

                if (!initializer)
                    throw RayTracer::Exception("Primitive initializer not found in plugins for type: " + key);
                
                auto nodePtr = std::make_unique<Node>();
                nodePtr->value.emplace<Object>(std::move(obj));
                std::shared_ptr<APrimitive> prim = std::get<std::shared_ptr<APrimitive>>(initializer(std::move(nodePtr)));
                this->addPrimitive(prim);
            }
        }
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
            if (nodePtr->value.index() == 1){
                std::function<RayTracer::Component(RayTracer::NodePtr)> initializer = pluginManager.getInitializer(key);

                if (!initializer)
                    throw RayTracer::Exception("Light initializer not found in plugins for type: " + key);

                std::shared_ptr<ILight> light = std::get<std::shared_ptr<ILight>>(initializer(std::move(nodePtr)));
                this->addLight(light);  
            } else {
                VectorObject &light = std::get<VectorObject>(nodePtr->value);
                for (RayTracer::Object &obj : light)
                {
                    std::function<RayTracer::Component(RayTracer::NodePtr)> initializer = pluginManager.getInitializer(key);
    
                    if (!initializer)
                        throw RayTracer::Exception("Light initializer not found in plugins for type: " + key);
                    
                    std::unique_ptr<Node> nodePtr = std::make_unique<Node>();
                    nodePtr->value.emplace<Object>(std::move(obj));
                    std::shared_ptr<ILight> light = std::get<std::shared_ptr<ILight>>(initializer(std::move(nodePtr)));
                    this->addLight(light);
                }
            }
        }
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
