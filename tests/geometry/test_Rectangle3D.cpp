#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../src/Geometry/Point3D/Point3D.hpp"
#include "../../src/Geometry/Vector3D/Vector3D.hpp"

using Geometry::Rectangle3D;
using Geometry::Point3D;
using Geometry::Vector3D;

static bool approx(double a, double b, double eps = 1e-6)
{
    return std::fabs(a - b) <= eps;
}

Test(Rectangle3D, default_rectangle_returns_origin)
{
    Rectangle3D r;
    Point3D p = r.pointAt(0.0, 0.0);
    cr_assert(approx(p.x, 0.0));
    cr_assert(approx(p.y, 0.0));
    cr_assert(approx(p.z, 0.0));

    // any u,v should also return origin because width/height default to zero
    Point3D p2 = r.pointAt(0.7, 0.3);
    cr_assert(approx(p2.x, 0.0));
}

Test(Rectangle3D, corners_map_correctly)
{
    Rectangle3D r;
    r.origin = Point3D(1.0, 2.0, 3.0);
    r.width = Vector3D(10.0, 0.0, 0.0);
    r.height = Vector3D(0.0, 5.0, 0.0);

    Point3D p00 = r.pointAt(0.0, 0.0);
    cr_assert(approx(p00.x, 1.0));

    Point3D p10 = r.pointAt(1.0, 0.0);
    cr_assert(approx(p10.x, 11.0));

    Point3D p01 = r.pointAt(0.0, 1.0);
    cr_assert(approx(p01.y, 7.0));

    Point3D p11 = r.pointAt(1.0, 1.0);
    cr_assert(approx(p11.x, 11.0));
    cr_assert(approx(p11.y, 7.0));
}

Test(Rectangle3D, mid_and_out_of_bounds_behaviour)
{
    Rectangle3D r;
    r.origin = Point3D(0.0, 0.0, 0.0);
    r.width = Vector3D(2.0, 0.0, 0.0);
    r.height = Vector3D(0.0, 4.0, 0.0);

    Point3D mid = r.pointAt(0.5, 0.5);
    cr_assert(approx(mid.x, 1.0));
    cr_assert(approx(mid.y, 2.0));

    // u/v outside [0,1] should extrapolate linearly
    Point3D extrap = r.pointAt(1.5, -0.5);
    cr_assert(approx(extrap.x, 3.0));
    cr_assert(approx(extrap.y, -2.0));
}
