/**
 * @file src/Geometry/Ray/Ray.cpp
 * @brief Implementation of the `Geometry::Ray` type.
 *
 * Implements the lightweight `Ray` structure used by renderers to represent
 * origins and directions for intersection queries. See @ref Geometry::Ray.
 */
#include "Ray.hpp"

namespace Geometry
{
    Point3D Ray::at(float t) const
    {
        return Point3D(
            origin.x + (direction.x * t),
            origin.y + (direction.y * t),
            origin.z + (direction.z * t));
    }
} // namespace Geometry