#pragma once
#include <math.h>

namespace Geometry
{
    /// Represents a 3D vector for directions or displacements.
    class Vector3D
    {
    public:
        double x;  ///< X component
        double y;  ///< Y component
        double z;  ///< Z component

        Vector3D(void) : x(0), y(0), z(0) {}
        Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
        ~Vector3D(void) = default;

        /// Calculate the length (magnitude) of this vector.
        double length(void) const;
        double length(void);

        /// Compute the dot product of two vectors.
        static double dot(const Vector3D &v1, const Vector3D &v2);
        double dot(const Vector3D &other) const;
        static Vector3D cross(const Vector3D &a, const Vector3D &b);
        Vector3D normalize() const;

        Vector3D operator+(const Vector3D &other) const;
        Vector3D operator+=(const Vector3D &other);
        Vector3D operator-(const Vector3D &other) const;
        Vector3D operator-=(const Vector3D &other);
        Vector3D operator*(const Vector3D &other) const;
        Vector3D operator*=(const Vector3D &other);
        Vector3D operator/(const Vector3D &other) const;
        Vector3D operator/=(const Vector3D &other);
        Vector3D operator*(double scalar) const;
        Vector3D operator*=(double scalar);
        Vector3D operator/(double scalar) const;
        Vector3D operator/=(double scalar);
        Vector3D operator=(const Vector3D &other);
    };
}
