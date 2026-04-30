#pragma once
#include "../../Interface/IMaterial.hpp"

namespace RayTracer
{
    class APrimitive
    {
    private:
        IMaterial *_material;

    public:
        APrimitive(IMaterial *material) : _material(material) {};
        virtual ~APrimitive() = default;
        virtual std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const = 0;
        IMaterial *getMaterial() const;
    };
} // namespace RayTracer
