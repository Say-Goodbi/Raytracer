#include "SceneSystem/ConfigParser/Parser.hpp"
#include <criterion/criterion.h>

Test(PARSING_FORMAT, variable_format)
{
    cr_assert_throw(RayTracer::Parser("tests/parsing/config/variable_format.cfg"), RayTracer::ParsingException);
}
