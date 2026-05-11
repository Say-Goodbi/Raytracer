#include "Scene.hpp"

namespace RayTracer
{
    Scene::Scene(ARenderer& camera) : _renderer(camera)
    {
    }

    void Scene::addPrimitive(std::unique_ptr<APrimitive> primitive)
    {
        this->_primitives.push_back(std::move(primitive));
    }

    std::vector<std::unique_ptr<APrimitive>>& Scene::getPrimitives()
    {
        return this->_primitives;
    }

    void Scene::addLight(std::unique_ptr<ILight> light)
    {
        this->_lights.push_back(std::move(light));
    }

    std::vector<std::unique_ptr<ILight>>& Scene::getLights()
    {
        return this->_lights;
    }

    const ARenderer& Scene::getRenderer() const
    {
        return this->_renderer;
    }

} // namespace RayTracer
