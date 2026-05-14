#include "Sphere.hpp"
#include <memory>

namespace RayTracer
{
    Sphere::Sphere(Geometry::Point3D center, float radius, std::shared_ptr<IMaterial> material)
        : APrimitive(std::move(material)), _center(center), _radius(radius)
    {
    }

    std::optional<Geometry::HitRecord> Sphere::hit(const Geometry::Ray& ray) const
    {
        Geometry::Vector3D oc = ray.origin - _center;
        float a = ray.direction.dot(ray.direction);
        float h = oc.dot(ray.direction);
        float c = oc.dot(oc) - _radius * _radius;
        float disc = h * h - a * c;

        if (disc >= 0) {
            float sqrtDisc = std::sqrt(disc);
            float t = (-h - sqrtDisc) / a;
            if (t <= 1e-4f)
                t = (-h + sqrtDisc) / a;
            if (t > 1e-4f) {
                Geometry::Point3D hitPoint = ray.at(t);
                Geometry::Vector3D normal = (hitPoint - _center).normalize();
                return Geometry::HitRecord(hitPoint, normal, t, this->getMaterial());
            }
        }
        return std::nullopt;
    }
} // namespace RayTracer
