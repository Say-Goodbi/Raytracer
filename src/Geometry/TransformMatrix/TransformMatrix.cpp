#include "TransformMatrix.hpp"

namespace Geometry
{
    TransformMatrix TransformMatrix::translation(float tx, float ty, float tz)
    {
        TransformMatrix result = TransformMatrix::identity();
        result.data[0][3] = tx;
        result.data[1][3] = ty;
        result.data[2][3] = tz;
        return result;
    }

    TransformMatrix TransformMatrix::scaling(float sx, float sy, float sz)
    {
        TransformMatrix result = TransformMatrix::identity();
        result.data[0][0] = sx;
        result.data[1][1] = sy;
        result.data[2][2] = sz;
        return result;
    }

    TransformMatrix TransformMatrix::rotationX(float angle)
    {
        TransformMatrix result = TransformMatrix::identity();
        float radians = angle * M_PI / 180.0f;
        float cosA = cos(radians);
        float sinA = sin(radians);
        result.data[1][1] = cosA;
        result.data[1][2] = sinA;
        result.data[2][1] = -sinA;
        result.data[2][2] = cosA;
        return result;
    }

    TransformMatrix TransformMatrix::rotationY(float angle)
    {
        TransformMatrix result = TransformMatrix::identity();
        float radians = angle * M_PI / 180.0f;
        float cosA = cos(radians);
        float sinA = sin(radians);
        result.data[0][0] = cosA;
        result.data[0][2] = -sinA;
        result.data[2][0] = sinA;
        result.data[2][2] = cosA;
        return result;
    }

    TransformMatrix TransformMatrix::rotationZ(float angle)
    {
        TransformMatrix result = TransformMatrix::identity();
        float radians = angle * M_PI / 180.0f;
        float cosA = cos(radians);
        float sinA = sin(radians);
        result.data[0][0] = cosA;
        result.data[0][1] = -sinA;
        result.data[1][0] = sinA;
        result.data[1][1] = cosA;
        return result;
    }

    Point3D TransformMatrix::operator*(const Point3D& point) const
    {
        Matrix<4, 1> homogeneousP;
        homogeneousP(0, 0) = point.x;
        homogeneousP(1, 0) = point.y;
        homogeneousP(2, 0) = point.z;
        homogeneousP(3, 0) = 1.0f;

        Matrix<4, 1> result = static_cast<const Matrix<4, 4>&>(*this) * homogeneousP;
        return Point3D(result(0, 0), result(1, 0), result(2, 0));
    }

    Vector3D TransformMatrix::operator*(const Vector3D& vector) const
    {
        Matrix<4, 1> homogeneousV;
        homogeneousV(0, 0) = vector.x;
        homogeneousV(1, 0) = vector.y;
        homogeneousV(2, 0) = vector.z;
        homogeneousV(3, 0) = 0.0f;

        Matrix<4, 1> result = static_cast<const Matrix<4, 4>&>(*this) * homogeneousV;
        return Vector3D(result(0, 0), result(1, 0), result(2, 0));
    }
}
