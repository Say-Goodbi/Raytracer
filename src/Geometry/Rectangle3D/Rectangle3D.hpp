#pragma once
#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"

namespace Geometry
{
    /// Represents a planar rectangular surface in 3D space.
    /// Defined by an origin and two edge vectors (width and height).
    class Rectangle3D
    {
    private:
    protected:
    public:
        Rectangle3D();
        Point3D origin;        ///< Rectangle origin
        Vector3D width;        ///< Width edge vector
        Vector3D height;       ///< Height edge vector

        /// Get a point on the rectangle at normalized coordinates (u, v).
        /// @param u Normalized u coordinate [0, 1]
        /// @param v Normalized v coordinate [0, 1]
        /// @return Point at origin + u*width + v*height
        Point3D pointAt(double u, double v) const;
    };
} // namespace Geometry
