#pragma once
#include "../../../../Abstracts/APrimitive/APrimitive.hpp"

namespace RayTracer
{
    class Sphere : public APrimitive
    {
        private:
            Geometry::Point3D _center;
            float _radius;
        public:
            Sphere(Geometry::Point3D center, float radius, IMaterial *material);
            ~Sphere() = default;
            std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const override;
    };
} // namespace RayTracer
