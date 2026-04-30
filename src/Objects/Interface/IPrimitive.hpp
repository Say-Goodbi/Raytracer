#pragma once
#include <optional>
#include "../../Geometry/Ray/Ray.hpp"
#include "../../Geometry/HitRecord/HitRecord.hpp"

namespace RayTracer
{
    class IPrimitive
    {
    public:
        virtual ~IPrimitive() = default;
        virtual std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const = 0;
        virtual IMaterial *getMaterial() const = 0;
    };
} // namespace RayTracer
