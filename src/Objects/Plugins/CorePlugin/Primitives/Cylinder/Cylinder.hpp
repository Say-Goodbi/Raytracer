#pragma once

#include "../../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include "../../../../../Objects/Abstracts/IMaterial.hpp"
#include "../../../../../Geometry/Ray/Ray.hpp"
#include <memory>
#include <optional>

namespace RayTracer
{
    class Cylinder : public APrimitive
    {
        private:
            Geometry::Point3D _origin;
            Geometry::Vector3D _axis;
            float _radius;
            float _height; // 0 = infinite cylinder
        public:
            Cylinder(const Geometry::Point3D& origin, const Geometry::Vector3D& axis, float radius, std::shared_ptr<IMaterial> material, float height = 0.0f) : APrimitive(std::move(material)), _origin(origin), _axis(axis), _radius(radius), _height(height) {}
            Cylinder() = default;
            ~Cylinder() = default;
            std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
            std::optional<Geometry::AABB> getBounds() const override;
    };
} // namespace RayTracer
