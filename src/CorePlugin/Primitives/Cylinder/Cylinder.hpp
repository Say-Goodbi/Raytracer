#pragma once

#include "../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include "../../../Objects/Abstracts/IMaterial.hpp"

namespace RayTracer
{
    class Cylinder : public APrimitive
    {
        private:
            Geometry::Point3D _origin;
            Geometry::Vector3D _axis;
            float _radius;
        public:
            Cylinder(const Geometry::Point3D& origin, const Geometry::Vector3D& axis, float radius, IMaterial *material) : APrimitive(material), _origin(origin), _axis(axis), _radius(radius) {}
            Cylinder() = default;
            ~Cylinder() = default;
            std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
    };
} // namespace RayTracer
