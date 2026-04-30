#pragma once

#include "../../Interface/IPrimitive.hpp"
#include "../../Interface/IMaterial.hpp"

namespace RayTracer
{
    class Plane : public IPrimitive
    {
    private:
        Geometry::Point3D _point;
        Geometry::Vector3D _normal;
        IMaterial *_material;
    public:
        Plane(const Geometry::Point3D& point, const Geometry::Vector3D& normal, IMaterial *material) : _point(point), _normal(normal), _material(material) {}
        Plane() = default;
        ~Plane() = default;
        std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
    };
} // namespace RayTracer
