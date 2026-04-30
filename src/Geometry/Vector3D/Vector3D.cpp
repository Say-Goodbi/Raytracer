#include "Vector3D.hpp"

namespace Geometry
{
    double Vector3D::length(void) const
    {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    double Vector3D::length(void)
    {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    double Vector3D::dot(const Vector3D &v1, const Vector3D &v2)
    {
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }

    double Vector3D::dot(const Vector3D &other) const
    {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    Vector3D Vector3D::cross(const Vector3D &a, const Vector3D &b)
    {
        return Vector3D(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }

    Vector3D Vector3D::normalize() const
    {
        double len = length();
        return Vector3D(x / len, y / len, z / len);
    }

    Vector3D Vector3D::operator+(const Vector3D &other) const
    {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D Vector3D::operator+=(const Vector3D &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3D Vector3D::operator-(const Vector3D &other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D Vector3D::operator-=(const Vector3D &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3D Vector3D::operator*(const Vector3D &other) const
    {
        return Vector3D(x * other.x, y * other.y, z * other.z);
    }

    Vector3D Vector3D::operator*=(const Vector3D &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vector3D Vector3D::operator/(const Vector3D &other) const
    {
        return Vector3D(x / other.x, y / other.y, z / other.z);
    }

    Vector3D Vector3D::operator/=(const Vector3D &other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vector3D Vector3D::operator*(double scalar) const
    {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }

    Vector3D Vector3D::operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3D Vector3D::operator/(double scalar) const
    {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }

    Vector3D Vector3D::operator/=(double scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
}
