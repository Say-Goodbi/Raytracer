#include "PointLight.hpp"
#include "../../../Geometry/Ray/Ray.hpp"
#include <algorithm>

namespace RayTracer
{
    PointLight::PointLight(Geometry::Point3D position, double intensity, Color color)
        : _position(position), _intensity(intensity), _color(color) {}

    /**
     * @brief Compute this light's contribution to a surface point.
     *
     * Algorithm:
     * 1. Compute direction and distance from hit point to light position
     * 2. Cast a shadow ray from hit point toward the light
     * 3. If any primitive blocks the ray before reaching the light, attenuate
     * 4. Otherwise return full diffuse contribution scaled by intensity
     *
     * @param hit The hit record of the surface point to shade
     * @param primitives Scene primitives used for occlusion testing
     * @return Color contribution from this light, attenuated if occluded
     */
    Color PointLight::computeLight(const Geometry::HitRecord& hit, const std::vector<std::unique_ptr<APrimitive>>& primitives) const
    {
        // 1. direction from hit point toward the light
        Geometry::Vector3D toLight = Geometry::Vector3D(
            _position.x - hit.point.x,
            _position.y - hit.point.y,
            _position.z - hit.point.z
        );
        double lightDistance = toLight.length();
        Geometry::Vector3D toLightNorm = toLight.normalize();

        // 2. shadow ray with epsilon offset to avoid self-intersection
        Geometry::Ray shadowRay(
            Geometry::Point3D(
                hit.point.x + toLightNorm.x * 0.001,
                hit.point.y + toLightNorm.y * 0.001,
                hit.point.z + toLightNorm.z * 0.001
            ),
            toLightNorm
        );

        // 3. occlusion test — check if any primitive blocks the light
        for (const auto& primitive : primitives) {
            auto obstruction = primitive->hit(shadowRay);
            if (obstruction && obstruction->rayDistance < lightDistance)
                return Color(0, 0, 0);
        }

        // 4. no obstacle — return full diffuse contribution
        double diffuse = std::max(0.0, hit.normal.dot(toLightNorm));
        return _color * _intensity * diffuse;
    }
} // namespace RayTracer