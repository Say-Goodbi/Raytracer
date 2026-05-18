#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/Vector3D/Vector3D.hpp"

using Geometry::Vector3D;

static bool approx(double a, double b, double eps = 1e-6)
{
	return std::fabs(a - b) <= eps;
}

Test(Vector3D, length_and_dot)
{
	Vector3D a(3.0, 4.0, 0.0);
	cr_assert(approx(a.length(), 5.0));

	Vector3D b(1.0, 0.0, 0.0);
	cr_assert(approx(Vector3D::dot(a, b), 3.0));
	cr_assert(approx(a.dot(b), 3.0));
}

Test(Vector3D, cross_and_normalize)
{
	Vector3D i(1.0, 0.0, 0.0);
	Vector3D j(0.0, 1.0, 0.0);
	Vector3D k = Vector3D::cross(i, j);
	cr_assert(approx(k.x, 0.0));
	cr_assert(approx(k.y, 0.0));
	cr_assert(approx(k.z, 1.0));

	Vector3D v(0.0, 0.0, 0.0);
	Vector3D norm = v.normalize();
	cr_assert(approx(norm.length(), 0.0));
}

Test(Vector3D, arithmetic_operators)
{
	Vector3D a(1.0, 2.0, 3.0);
	Vector3D b(2.0, 3.0, 4.0);

	Vector3D c = a + b;
	cr_assert(approx(c.x, 3.0));
	cr_assert(approx(c.y, 5.0));
	cr_assert(approx(c.z, 7.0));

	Vector3D d = b - a;
	cr_assert(approx(d.x, 1.0));

	Vector3D e = a * b; // element-wise
	cr_assert(approx(e.x, 2.0));

	Vector3D f = b / Vector3D(2.0, 1.5, 4.0);
	cr_assert(approx(f.x, 1.0));

	Vector3D g = a * 2.0;
	cr_assert(approx(g.x, 2.0));

	a += Vector3D(1.0, 1.0, 1.0);
	cr_assert(approx(a.x, 2.0));

	a *= 0.5;
	cr_assert(approx(a.x, 1.0));
}

