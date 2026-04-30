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
