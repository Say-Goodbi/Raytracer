#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "ILight.hpp"
#include <vector>

namespace RayTracer
{
    /// Abstract base for materials defining how surfaces shade.
    class IMaterial
    {
    public:
        /// Compute the color at a surface hit point.
        /// @param hit The hit record with intersection data
        /// @param lights Lights in the scene
        /// @return Computed color at the hit point
        virtual Color computeColor(Geometry::HitRecord& hit, std::vector<ILight*>& lights) = 0;
        virtual ~IMaterial() = default;
    };
} // namespace RayTracer
