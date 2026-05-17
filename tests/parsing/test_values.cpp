#include "SceneSystem/ConfigParser/Parser.hpp"
#include "SceneSystem/SceneNodeHelpers/SceneNodeHelpers.hpp"
#include <criterion/criterion.h>

Test(PARSING_VALUES, parse_fieldOfView)
{
    RayTracer::Parser parser("tests/parsing/config/simple.cfg");
    const RayTracer::Object &root = std::get<RayTracer::Object>(parser._node->value);
    const RayTracer::NodePtr &cameraPtr = root.at("camera");
    const RayTracer::Object &cameraObj = std::get<RayTracer::Object>(cameraPtr->value);
    float fov = Raytracer::fromNode<float>(Raytracer::Detail::field(cameraObj, "fieldOfView"));
    cr_assert_float_eq(fov, 72.0f, 1e-6);
}

Test(PARSING_VALUES, parse_planes_count)
{
    RayTracer::Parser parser("tests/parsing/config/simple.cfg");
    const RayTracer::Object &root = std::get<RayTracer::Object>(parser._node->value);
    const RayTracer::NodePtr &primitivesPtr = root.at("primitives");
    const RayTracer::Object &primitivesObj = std::get<RayTracer::Object>(primitivesPtr->value);
    const RayTracer::NodePtr &planesPtr = Raytracer::Detail::field(primitivesObj, "planes");
    const RayTracer::VectorObject &planesVec = std::get<RayTracer::VectorObject>(planesPtr->value);
    cr_assert_eq(planesVec.size(), 1u);
}

Test(PARSING_VALUES, object_values)
{
    RayTracer::Parser parser("tests/parsing/config/simple.cfg");
    const RayTracer::Object &root = std::get<RayTracer::Object>(parser._node->value);
    const RayTracer::NodePtr &cameraPtr = root.at("camera");
    const RayTracer::Object &cameraObj = std::get<RayTracer::Object>(cameraPtr->value);
    const Geometry::Point3D position = Raytracer::fromNode<Geometry::Point3D>(Raytracer::Detail::field(cameraObj, "position"));
    cr_assert_float_eq(position.x, 0.0f, 1e-6);
    cr_assert_float_eq(position.y, -100.0f, 1e-6);
    cr_assert_float_eq(position.z, 20.0f, 1e-6);
}
