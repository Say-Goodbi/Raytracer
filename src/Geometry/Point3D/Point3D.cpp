#include "Point3D.hpp"

namespace Geometry
{
    Point3D Point3D::operator+(const Vector3D &v) const
    {
        return Point3D(x + v.x, y + v.y, z + v.z);
    }

    Point3D Point3D::operator-(const Vector3D &v) const
    {
        return Point3D(x - v.x, y - v.y, z - v.z);
    }

    Point3D &Point3D::operator+=(const Vector3D &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3D Point3D::operator-(const Point3D &other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }
}
