#include "Ray.hpp"

namespace Geometry
{
    Point3D Ray::at(float t)
    {
        return Point3D(
            origin.x + (direction.x * t),
            origin.y + (direction.y * t),
            origin.z + (direction.z * t));
    }
} // namespace Geometry