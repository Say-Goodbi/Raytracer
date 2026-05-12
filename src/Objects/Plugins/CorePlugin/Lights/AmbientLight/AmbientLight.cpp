#include "AmbientLight.hpp"

namespace RayTracer
{
    AmbientLight::AmbientLight(float intensity, Color color)
        : _intensity(intensity), _color(color) {}

    Color AmbientLight::computeLight(const Geometry::HitRecord &hit, const std::vector<std::shared_ptr<APrimitive>> &primitives) const
    {
        return _color * _intensity;
    }
} // namespace RayTracer