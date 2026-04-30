#include "Camera.hpp"
#include "../Scene/Scene.hpp"
#include "../../Objects/Interface/IPrimitive.hpp"
#include <vector>

namespace RayTracer
{
    Geometry::Ray Camera::generateRay(double u, double v)
    {
        Geometry::Point3D screenPoint = this->_screen.pointAt(u, v);
        Geometry::Vector3D rayDirection = Geometry::Vector3D(
            screenPoint.x - this->_position.x,
            screenPoint.y - this->_position.y,
            screenPoint.z - this->_position.z).normalize();
        return Geometry::Ray(this->_position, rayDirection);
    }
    
    Color Camera::castRay(Geometry::Ray& ray, Scene& scene)
    {
        std::optional<Geometry::HitRecord> closest;
        IPrimitive* hitPrimitive = nullptr;

        for (const auto &primitive : scene.getPrimitives()) {
            std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
            if (hit && (!closest || hit->rayDistance < closest->rayDistance)) {
                closest = hit;
                hitPrimitive = primitive;
            }
        }

        if (!closest)
            return Color(0, 0, 0);

        bool inShadow = computeShadow(closest.value(), scene);
        
        Color finalColor = hitPrimitive->getMaterial()->computeColor(closest.value(), scene.getLights());

        if (inShadow)
            finalColor = finalColor * 0.1f;

        return finalColor.clamp();
    }
    
    bool Camera::computeShadow(Geometry::HitRecord &hit, Scene &scene)
    {
        for (const auto &light : scene.getLights()) {
            Geometry::Vector3D toLight = light->getPosition() - hit.point;
            float distanceToLight = toLight.length();
            Geometry::Vector3D direction = toLight.normalize();

            Geometry::Point3D shadowOrigin = hit.point + direction * 0.001f;
            Geometry::Ray shadowRay(shadowOrigin, direction);
            for (const auto &primitive : scene.getPrimitives()) {
                std::optional<Geometry::HitRecord> shadowHit = primitive->hit(shadowRay);
                if (shadowHit && shadowHit->rayDistance < distanceToLight)
                    return true;
            }
        }
        return false;
    }
    
    std::vector<std::vector<Color>> Camera::render(Scene& scene)
    {
        std::vector<std::vector<Color>> frameBuffer;
        frameBuffer.resize(_height, std::vector<Color>(_width));

        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                double u = static_cast<double>(x) / (_width - 1);
                double v = static_cast<double>(y) / (_height - 1);
                Geometry::Ray ray = generateRay(u, v);
                frameBuffer[y][x] = castRay(ray, scene);
            }
        }

        return frameBuffer;
    }

} // namespace RayTracer
