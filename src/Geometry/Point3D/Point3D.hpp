#pragma once

#include "../Vector3D/Vector3D.hpp"

namespace Geometry
{
    class Point3D
    {
    public:
        double x;
        double y;
        double z;

        Point3D(void) : x(0), y(0), z(0) {}
        Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
        ~Point3D(void) = default;

        Point3D operator+(const Vector3D &v) const;
        Point3D operator-(const Vector3D &v) const;
        Point3D &operator+=(const Vector3D &v);
        Vector3D operator-(const Point3D &other) const;
    };
}
