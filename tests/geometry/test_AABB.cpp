#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/AABB/AABB.hpp"
#include "../../src/Geometry/Ray/Ray.hpp"

using Geometry::AABB;
using Geometry::Point3D;
using Geometry::Ray;
using Geometry::Vector3D;

static bool approxf(double a, double b, double eps = 1e-6)
{
    return std::fabs(a - b) <= eps;
}

Test(AABB, unite_and_centroid_and_surface)
{
    AABB a(Point3D(0,0,0), Point3D(1,1,1));
    AABB b(Point3D(-1,-1,-1), Point3D(0.5,0.5,0.5));
    AABB u = AABB::unite(a,b);
    cr_assert(u.minimum.x < 0.0);
    cr_assert(u.maximum.x > 0.5);

    Point3D c = u.centroid();
    cr_assert(std::isfinite(c.x));

    double surface = a.surfaceArea();
    cr_assert(surface > 0.0);

    AABB deg(Point3D(0,0,0), Point3D(0,0,0));
    cr_assert(approxf(deg.surfaceArea(), 0.0));
}

Test(AABB, hit_basic_and_edge_cases)
{
    AABB box(Point3D(-1,-1,-1), Point3D(1,1,1));
    Ray r1(Point3D(0,0,-5), Vector3D(0,0,1));
    cr_assert(box.hit(r1));

    Ray r2(Point3D(2,2,2), Vector3D(1,0,0));
    cr_assert(!box.hit(r2));

    // Ray parallel to X axis but inside YZ slab
    Ray r3(Point3D(0,0,0), Vector3D(0,1,0));
    cr_assert(box.hit(r3));
}
