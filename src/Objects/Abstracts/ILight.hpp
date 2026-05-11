#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include <vector>
#include <memory>

namespace RayTracer
{
    class APrimitive;
    class ILight
    {
    public:
        virtual ~ILight() = default;
        /// Compute this light's contribution to a surface.
        /// Handles both direct illumination and shadow testing internally.
        /// @param hit The hit record
        /// @param primitives Scene primitives for occlusion tests
        /// @return Color contribution from this light, attenuated if occluded
        virtual Color computeLight(const Geometry::HitRecord& hit, const std::vector<std::unique_ptr<APrimitive>>& primitives) const = 0;
    };
} // namespace RayTracer