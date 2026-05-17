#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/TransformMatrix/TransformMatrix.hpp"
#include "../../src/Geometry/Point3D/Point3D.hpp"
#include "../../src/Geometry/Vector3D/Vector3D.hpp"

using Geometry::TransformMatrix;
using Geometry::Point3D;
using Geometry::Vector3D;

static bool approxd(double a, double b, double eps = 1e-6)
{
    return std::fabs(a - b) <= eps;
}

Test(TransformMatrix, translation_moves_point)
{
    TransformMatrix t = TransformMatrix::translation(1.0f, 2.0f, 3.0f);
    Point3D p(0.0, 0.0, 0.0);
    Point3D tp = t * p;
    cr_assert(approxd(tp.x, 1.0));
}

Test(TransformMatrix, scaling_changes_vector_components)
{
    TransformMatrix s = TransformMatrix::scaling(2.0f, 3.0f, 4.0f);
    Vector3D v(1.0, 1.0, 1.0);
    Vector3D sv = s * v;
    cr_assert(approxd(sv.x, 2.0));
    cr_assert(approxd(sv.y, 3.0));
}

Test(TransformMatrix, rotationZ_turns_x_to_y)
{
    // rotationZ 90 degrees: (1,0,0) -> (0,1,0)
    TransformMatrix rz = TransformMatrix::rotationZ(90.0f);
    Vector3D x(1.0, 0.0, 0.0);
    Vector3D rx = rz * x;
    cr_assert(approxd(rx.x, 0.0, 1e-5));
    cr_assert(approxd(rx.y, 1.0, 1e-5));
}

Test(TransformMatrix, translation_does_not_affect_vectors)
{
    Point3D p(1.0, 0.0, 0.0);
    TransformMatrix tr = TransformMatrix::translation(5.0f, 0.0f, 0.0);
    Vector3D x(1.0, 0.0, 0.0);
    Vector3D vt = tr * x;
    cr_assert(approxd(vt.x, 1.0));
}
