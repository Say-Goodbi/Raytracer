#include <criterion/criterion.h>
#include <criterion/redirect.h>

Test(sample_test, should_pass)
{
    cr_assert_eq(1, 1, "Expected 1 to be equal to 1");
}