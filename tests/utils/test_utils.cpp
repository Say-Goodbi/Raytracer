#include "Utils/Color.hpp"
#include "Utils/Utils.hpp"
#include <criterion/criterion.h>

Test(UTILS, color_clamp_limits_channels)
{
    RayTracer::Color color(1.4, -0.2, 0.5);
    color.clamp();

    cr_assert_float_eq(color.r, 1.0, 1e-6);
    cr_assert_float_eq(color.g, 0.0, 1e-6);
    cr_assert_float_eq(color.b, 0.5, 1e-6);
}

Test(UTILS, rng_stays_in_unit_interval)
{
    for (int i = 0; i < 32; ++i)
    {
        const double value = RayTracer::Utils::rng();
        cr_assert_geq(value, 0.0);
        cr_assert_lt(value, 1.0);
    }
}
