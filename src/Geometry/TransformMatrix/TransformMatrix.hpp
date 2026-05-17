/**
 * @file src/Geometry/TransformMatrix/TransformMatrix.hpp
 * @brief Affine transform matrix utilities used for camera and object transforms.
 *
 * Declares `Geometry::TransformMatrix`, a 4x4 affine matrix type built on
 * `Geometry::Matrix<4,4>` that provides constructors for translation,
 * scaling and rotation. It offers convenience operators to transform
 * `Point3D` and `Vector3D` values in world and object space.
 */
#pragma once
#include "../Matrix/Matrix.hpp"
#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"

namespace Geometry
{
    class TransformMatrix : public Geometry::Matrix<4, 4>
    {
        public:
            TransformMatrix(const Geometry::Matrix<4, 4>& other) : Geometry::Matrix<4, 4>(other) {}
            TransformMatrix() : Geometry::Matrix<4, 4>() {}
            ~TransformMatrix() = default;

            static TransformMatrix translation(float tx, float ty, float tz);
            static TransformMatrix scaling(float sx, float sy, float sz);
            static TransformMatrix rotationX(float angle);
            static TransformMatrix rotationY(float angle);
            static TransformMatrix rotationZ(float angle);

            Point3D operator*(const Point3D& point) const;
            Vector3D operator*(const Vector3D& vector) const;
    };
}