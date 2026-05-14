#pragma once

#include "../../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include "../../../../../Objects/Abstracts/IMaterial.hpp"
#include "../../../../../Geometry/Ray/Ray.hpp"
#include <memory>
#include <optional>

namespace RayTracer
{
    class Plane : public APrimitive
    {
        private:
            Geometry::Point3D _point;
            Geometry::Vector3D _normal;
        public:
            Plane(const Geometry::Point3D& point, const Geometry::Vector3D& normal, std::shared_ptr<IMaterial> material) : APrimitive(std::move(material)), _point(point), _normal(normal) {}
            Plane() = default;
            ~Plane() = default;
            std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
            std::optional<Geometry::AABB> getBounds() const override;
    };
} // namespace RayTracer
