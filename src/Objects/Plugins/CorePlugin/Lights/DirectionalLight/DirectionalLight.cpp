#include "DirectionalLight.hpp"
#include "../../../Geometry/Ray/Ray.hpp"
#include "../../../Geometry/HitRecord/HitRecord.hpp"
#include "../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <algorithm>

namespace RayTracer
{
    DirectionalLight::DirectionalLight(Geometry::Vector3D direction, double intensity, Color color)
        : _direction(direction.normalize()), _intensity(intensity), _color(color) {}

    /// @brief Compute light contribution with shadow testing.
    /// Shadow ray is cast in the opposite direction of the light.
    /// No distance limit since the light is infinitely far away.
    Color DirectionalLight::computeLight(
        const Geometry::HitRecord& hit, const std::vector<std::unique_ptr<APrimitive>>& primitives) const
    {
        // shadow ray in opposite direction of the light
        Geometry::Vector3D toLight = Geometry::Vector3D(
            -_direction.x,
            -_direction.y,
            -_direction.z
        );

        Geometry::Ray shadowRay(
            Geometry::Point3D(
                hit.point.x + toLight.x * 0.001,
                hit.point.y + toLight.y * 0.001,
                hit.point.z + toLight.z * 0.001
            ),
            toLight
        );

        // occlusion test — no distance limit, light is far
        for (const auto& primitive : primitives) {
            auto obstruction = primitive->hit(shadowRay);
            if (obstruction)
                return Color(0, 0, 0); // fully occluded, ambient handles the base intensity
        }

        double diffuse = std::max(0.0, hit.normal.dot(toLight));
        return _color * _intensity * diffuse;
    }
} // namespace RayTracer