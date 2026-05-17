#pragma once

#include "../../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include "../../../../../Objects/Abstracts/IMaterial.hpp"
#include "../../../../../Geometry/Ray/Ray.hpp"
#include <memory>
#include <optional>

namespace RayTracer
{
    class Cone : public APrimitive
    {
        private:
            Geometry::Point3D _apex;
            Geometry::Vector3D _axis;
            float _radius;
            float _height;
        public:
            Cone(const Geometry::Point3D& apex, const Geometry::Vector3D& axis, float radius, std::shared_ptr<IMaterial> material, float height = 0.0f) : APrimitive(std::move(material)), _apex(apex), _axis(axis), _radius(radius), _height(height) {}
            Cone() = default;
            ~Cone() = default;
            std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
            std::optional<Geometry::AABB> getBounds() const override;
    };
} // namespace RayTracer
