#include "Rectangle3D.hpp"

namespace Geometry
{
    Rectangle3D::Rectangle3D() : origin(), width(), height() {}

    Point3D Rectangle3D::pointAt(double u, double v) const
    {
        return Point3D(
            origin.x + (width.x * u) + (height.x * v),
            origin.y + (width.y * u) + (height.y * v),
            origin.z + (width.z * u) + (height.z * v));
    }
} // namespace Geometry
