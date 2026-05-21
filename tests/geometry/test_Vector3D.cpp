#include <criterion/criterion.h>
#include <cmath>

#include <iostream>
#include "../../src/Geometry/Vector3D/Vector3D.hpp"

using Geometry::Vector3D;

static bool approx(double a, double b, double eps = 1e-6)
{
	return std::fabs(a - b) <= eps;
}

Test(Vector3D, length_nonconst)
{
	Vector3D a(3.0, 4.0, 0.0);
	cr_assert(approx(a.length(), 5.0));
}

Test(Vector3D, length_const)
{
	const Vector3D constA(6.0, 8.0, 0.0);
	cr_assert(approx(constA.length(), 10.0));
}


Test(Vector3D, dot_and_cross)
{
	Vector3D a(3.0, 4.0, 0.0);
	Vector3D b(1.0, 0.0, 0.0);
	cr_assert(approx(Vector3D::dot(a, b), 3.0));
	cr_assert(approx(a.dot(b), 3.0));

	Vector3D i(1.0, 0.0, 0.0);
	Vector3D j(0.0, 1.0, 0.0);
	Vector3D k = Vector3D::cross(i, j);
	cr_assert(approx(k.x, 0.0));
	cr_assert(approx(k.y, 0.0));
	cr_assert(approx(k.z, 1.0));
}

Test(Vector3D, normalize_zero_vector_stays_zero_length)
{
	Vector3D v(0.0, 0.0, 0.0);
	Vector3D norm = v.normalize();
	cr_assert(approx(norm.length(), 0.0));
}

Test(Vector3D, vector_arithmetic)
{
	Vector3D a(1.0, 2.0, 3.0);
	Vector3D b(2.0, 3.0, 4.0);

	Vector3D c = a + b;
	cr_assert(approx(c.x, 3.0));
	cr_assert(approx(c.y, 5.0));
	cr_assert(approx(c.z, 7.0));

	Vector3D d = b - a;
	cr_assert(approx(d.x, 1.0));
	cr_assert(approx(d.y, 1.0));
	cr_assert(approx(d.z, 1.0));

	Vector3D e = a * b; // element-wise
	cr_assert(approx(e.x, 2.0));
	cr_assert(approx(e.y, 6.0));
	cr_assert(approx(e.z, 12.0));

	Vector3D f = b / Vector3D(2.0, 1.5, 4.0);
	cr_assert(approx(f.x, 1.0));
	cr_assert(approx(f.y, 2.0));
	cr_assert(approx(f.z, 1.0));
}

Test(Vector3D, scalar_arithmetic)
{
	Vector3D a(1.0, 2.0, 3.0);

	Vector3D g = a * 2.0;
	cr_assert(approx(g.x, 2.0));
	cr_assert(approx(g.y, 4.0));
	cr_assert(approx(g.z, 6.0));

	Vector3D h = g / 2.0;
	cr_assert(approx(h.x, 1.0));
	cr_assert(approx(h.y, 2.0));
	cr_assert(approx(h.z, 3.0));
}

Test(Vector3D, compound_assignment)
{
	Vector3D a(1.0, 2.0, 3.0);

	a += Vector3D(1.0, 1.0, 1.0);
	std::cout << "a: (" << a.x << ", " << a.y << ", " << a.z << ")\n";
	cr_assert(approx(a.x, 2.0));
	cr_assert(approx(a.y, 3.0));
	cr_assert(approx(a.z, 4.0));

	a -= Vector3D(1.0, 1.0, 1.0);
	cr_assert(approx(a.x, 1.0));
	cr_assert(approx(a.y, 2.0));
	cr_assert(approx(a.z, 3.0));

	a *= Vector3D(2.0, 3.0, 4.0);
	cr_assert(approx(a.x, 2.0));
	cr_assert(approx(a.y, 6.0));
	cr_assert(approx(a.z, 12.0));

	a /= Vector3D(2.0, 3.0, 4.0);
	cr_assert(approx(a.x, 1.0));
	cr_assert(approx(a.y, 2.0));
	cr_assert(approx(a.z, 3.0));

	a *= 0.5;
	cr_assert(approx(a.x, 0.5));
	cr_assert(approx(a.y, 1.0));
	cr_assert(approx(a.z, 1.5));

	a /= 0.5;
	cr_assert(approx(a.x, 1.0));
	cr_assert(approx(a.y, 2.0));
	cr_assert(approx(a.z, 3.0));
}

Test(Vector3D, copy_assignment)
{
	Vector3D source(2.0, 2.0, 3.0);
	Vector3D assigned;
	assigned = source;
	cr_assert(approx(assigned.x, 2.0));
	cr_assert(approx(assigned.y, 2.0));
	cr_assert(approx(assigned.z, 3.0));
}

