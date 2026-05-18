#include "SceneSystem/ConfigParser/Parser.hpp"
#include <criterion/criterion.h>

Test(PARSING_FILE, file_not_found)
{
    cr_assert_throw(RayTracer::Parser("non_existent_file.cfg"), RayTracer::ParsingException);
}

Test(PARSING_FILE, simple)
{
    cr_assert_no_throw(RayTracer::Parser("tests/parsing/config/simple.cfg"));
}

Test(PARSING_FILE, empty_file)
{
    cr_assert_throw(RayTracer::Parser("tests/parsing/config/empty.cfg"), RayTracer::ParsingException);
}
