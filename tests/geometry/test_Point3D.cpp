#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/Point3D/Point3D.hpp"
#include "../../src/Geometry/Vector3D/Vector3D.hpp"

using Geometry::Point3D;
using Geometry::Vector3D;

static bool approx(double a, double b, double eps = 1e-6)
{
    return std::fabs(a - b) <= eps;
}

Test(Point3D, add_and_subtract_vector)
{
    Point3D p(1.0, 2.0, 3.0);
    Vector3D v(0.5, 1.0, -1.0);

    Point3D q = p + v;
    cr_assert(approx(q.x, 1.5));

    Point3D r = q - v;
    cr_assert(approx(r.y, 2.0));

    Vector3D diff = r - p;
    cr_assert(approx(diff.x, 0.0));
}

Test(Point3D, inplace_and_assign)
{
    Point3D p(0.0, 0.0, 0.0);
    Vector3D v(1.0, 2.0, 3.0);
    p += v;
    cr_assert(approx(p.z, 3.0));

    Point3D s;
    s = p;
    cr_assert(approx(s.x, 1.0));
}
