#include "Plane.hpp"

namespace RayTracer
{
    std::optional<Geometry::HitRecord> Plane::hit(const Geometry::Ray& ray) const
    {
        float denom = _normal.dot(ray.direction);
        if (std::abs(denom) > 1e-6) {
            float t = (_point - ray.origin).dot(_normal) / denom;
            if (t >= 0) {
                Geometry::HitRecord hit(ray.at(t), _normal, t, this->_material);
                return hit;
            }
        }
        return std::nullopt;
    }
} // namespace RayTracer