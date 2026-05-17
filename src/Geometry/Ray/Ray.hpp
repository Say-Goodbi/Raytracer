/**
 * @file src/Geometry/Ray/Ray.hpp
 * @brief Ray primitive used to represent a parametric line (origin + direction).
 *
 * Declares `Geometry::Ray`, a small POD containing an origin `Point3D` and a
 * normalized direction `Vector3D`. Rays are the primary input to intersection
 * queries via @ref RayTracer::Scene::hit.
 */
#pragma once
#include "../../Geometry/Point3D/Point3D.hpp"
#include "../../Geometry/Vector3D/Vector3D.hpp"

namespace Geometry
{
    /// Represents a ray: an origin point and a direction vector.
    class Ray
    {
    private:
    protected:
    public:
        Ray() = default;
        ~Ray() = default;

        /// Constructor.
        /// @param origin Ray origin point
        /// @param direction Ray direction (typically normalized)
        Ray(const Geometry::Point3D& origin, const Geometry::Vector3D& direction) : origin(origin), direction(direction) {}
        
        /// Compute a point at parameter t along the ray.
        /// @param t Distance along the ray
        /// @return Point at origin + t * direction
        Point3D at(float t) const;
        
        Geometry::Point3D origin;      ///< Ray origin
        Geometry::Vector3D direction;  ///< Ray direction
    };
} // namespace Geometry
