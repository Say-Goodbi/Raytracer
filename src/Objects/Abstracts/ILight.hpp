#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "../../SceneSystem/Scene/Scene.hpp"

namespace RayTracer
{
    /// Abstract base for light sources.
    class ILight
    {
    public:
        virtual ~ILight() = default;

        /// Compute this light's contribution to a surface.
        /// @param hit The hit record
        /// @param scene The scene (for occlusion tests, etc.)
        /// @return Color contribution from this light
        virtual Color computeContribution(Geometry::HitRecord& hit, Scene& scene) = 0;

        /// Get the position of this light.
        /// @return Light position as a point in space
        virtual Geometry::Point3D getPosition() const = 0;
    };
} // namespace RayTracer
