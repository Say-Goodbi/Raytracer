#include <criterion/criterion.h>
#include <cmath>

#include "../../src/Geometry/Matrix/Matrix.hpp"

using Geometry::Matrix;

static bool approxf(float a, float b, float eps = 1e-6f)
{
    return std::fabs(a - b) <= eps;
}

Test(Matrix, construct_and_identity)
{
    Matrix<2,2> m;
    cr_assert(approxf(m(0,0), 0.0f));

    auto id = Matrix<2,2>::identity();
    cr_assert(approxf(id(0,0), 1.0f));
    cr_assert(approxf(id(0,1), 0.0f));
    cr_assert(approxf(id(1,0), 0.0f));
    cr_assert(approxf(id(1,1), 1.0f));
}

Test(Matrix, add_subtract_and_scalar)
{
    float aData[2][2] = {{1.0f,2.0f},{3.0f,4.0f}};
    float bData[2][2] = {{0.5f,1.0f},{1.5f,2.0f}};
    Matrix<2,2> A(aData);
    Matrix<2,2> B(bData);

    Matrix<2,2> C = A + B;
    cr_assert(approxf(C(0,0), 1.5f));

    A += B;
    cr_assert(approxf(A(1,1), 6.0f));

    Matrix<2,2> S = B * 2.0f;
    cr_assert(approxf(S(0,0), 1.0f));
}

Test(Matrix, multiply)
{
    float aData[2][3] = {{1,2,3},{4,5,6}};
    float bData[3][2] = {{7,8},{9,10},{11,12}};
    Matrix<2,3> A(aData);
    Matrix<3,2> B(bData);

    auto R = A * B; // should be 2x2
    cr_assert(approxf(R(0,0), 58.0f));
    cr_assert(approxf(R(1,1), 154.0f));
}
