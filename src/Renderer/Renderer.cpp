#include "Renderer.hpp"

using namespace Geometry;
namespace RayTracer
{
    Renderer::Renderer(int width, int height) : _width(width), _height(height)
    {
        _frameBuffer.resize(_height, std::vector<Color>(_width));
    }

    Color Renderer::castRay(Geometry::Ray& ray, Scene& scene)
    {
        std::optional<HitRecord> closest;
        IPrimitive* hitPrimitive = nullptr;

        for (const auto &primitive : scene.getPrimitives()) {
            std::optional<HitRecord> hit = primitive->hit(ray);
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

    bool Renderer::computeShadow(Geometry::HitRecord &hit, Scene &scene)
    {
        for (const auto &light : scene.getLights()) {
            Vector3D toLight = light->getPosition() - hit.point;
            float distanceToLight = toLight.length();
            Vector3D direction = toLight.normalize();

            Point3D shadowOrigin = hit.point + direction * 0.001f;
            Ray shadowRay(shadowOrigin, direction);
            for (const auto &primitive : scene.getPrimitives()) {
                std::optional<HitRecord> shadowHit = primitive->hit(shadowRay);
                if (shadowHit && shadowHit->rayDistance < distanceToLight)
                    return true;
            }
        }
        return false;
    }

    void Renderer::writeOutput()
    {
        for (unsigned int i = 0; i < _height; ++i)
            for (unsigned int j = 0; j < _width; ++j) {
                const Color &color = _frameBuffer[i][j];
                std::cout << static_cast<int>(color.r * 255) << " "
                          << static_cast<int>(color.g * 255) << " "
                          << static_cast<int>(color.b * 255) << "\n";
            }
    }
} // namespace RayTracer