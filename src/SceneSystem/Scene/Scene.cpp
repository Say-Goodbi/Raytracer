/**
 * @file src/SceneSystem/Scene/Scene.cpp
 * @brief Implementation of the `RayTracer::Scene` container and utilities.
 *
 * The `Scene` owns primitives, lights and the renderer instance. It provides
 * acceleration structure preparation (`BVH`) and the `hit()` interface used by
 * the renderer to perform ray-primitive intersection queries. See
 * @ref RayTracer::Scene and @ref RayTracer::ARenderer for integration points.
 */
#include "Scene.hpp"

namespace
{
    std::string getStringScalar(const RayTracer::NodePtr &node)
    {
        if (!node || !std::holds_alternative<RayTracer::ScalarValue>(node->value))
            throw RayTracer::Exception("Expected a scalar node");

        const RayTracer::ScalarValue &value = std::get<RayTracer::ScalarValue>(node->value);
        if (!std::holds_alternative<std::string>(value))
            throw RayTracer::Exception("Expected a string scalar value");

        return std::get<std::string>(value);
    }

    std::optional<Geometry::HitRecord> findClosestLinear(const Geometry::Ray &ray, const std::vector<std::shared_ptr<RayTracer::APrimitive>> &primitives)
    {
        std::optional<Geometry::HitRecord> closest;

        for (const auto &primitive : primitives)
        {
            std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
            if (hit && (!closest || hit->rayDistance < closest->rayDistance))
                closest = hit;
        }
        return closest;
    }
}

namespace RayTracer
{
    Scene::Scene(ARenderer &camera) : _renderer(&camera, [](ARenderer *) {})
    {
    }

    Scene::Scene(void) : _renderer(nullptr) {}

    void Scene::updateCamera(RayTracer::NodePtr &camera, PluginManager &pluginManager)
    {
        std::string type = std::get<Object>(camera->value)["type"]
            ? getStringScalar(std::get<Object>(camera->value)["type"])
            : "camera";
        auto initializer = pluginManager.getInitializer(type);

        if (!initializer)
            throw RayTracer::Exception(type + " initializer not found in plugins.");
        this->_renderer = std::get<std::shared_ptr<ARenderer>>(initializer(std::move(camera)));
    }

    void Scene::addPrimitive(std::shared_ptr<APrimitive> primitive)
    {
        this->_primitives.push_back(primitive);
    }

    void Scene::prepareAccelerationStructure()
    {
        this->_bvh.reset();
        this->_unboundedPrimitives.clear();

        std::vector<std::shared_ptr<APrimitive>> boundedPrimitives;
        boundedPrimitives.reserve(this->_primitives.size());

        for (const auto &primitive : this->_primitives)
        {
            if (primitive->getBounds())
                boundedPrimitives.push_back(primitive);
            else
                this->_unboundedPrimitives.push_back(primitive);
        }

        this->_bvh.emplace(boundedPrimitives);
    }

    std::optional<Geometry::HitRecord> Scene::hit(const Geometry::Ray &ray) const
    {
        std::optional<Geometry::HitRecord> closest;

        if (this->_bvh)
            closest = this->_bvh->hit(ray);

        std::optional<Geometry::HitRecord> linearHit = findClosestLinear(ray, this->_unboundedPrimitives);
        if (linearHit && (!closest || linearHit->rayDistance < closest->rayDistance))
            closest = linearHit;

        return closest;
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

    std::optional<std::reference_wrapper<const BVH>> Scene::getBVH() const
    {
        if (this->_bvh)
            return std::cref(*this->_bvh);
        return std::nullopt;
    }


} // namespace RayTracer
