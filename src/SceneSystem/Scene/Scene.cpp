#include "Scene.hpp"

namespace RayTracer
{
    Scene::Scene(Camera camera) : _camera(camera)
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

    const Camera& Scene::getCamera() const
    {
        return this->_camera;
    }

} // namespace RayTracer
