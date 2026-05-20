#include "SceneSystem/ConfigParser/Parser.hpp"
#include "SceneSystem/PluginManager/PluginManager.hpp"
#include "SceneSystem/SceneLoader/SceneLoader.hpp"
#include "SceneSystem/SceneNodeHelpers/SceneNodeHelpers.hpp"
#include "Geometry/Ray/Ray.hpp"
#include <criterion/criterion.h>
#include <filesystem>
#include <fstream>
#include <variant>

namespace
{
    std::filesystem::path makePluginSandbox()
    {
        std::filesystem::path sandbox = std::filesystem::temp_directory_path() / ("raytracer-scene-system-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        std::filesystem::create_directories(sandbox);
        return sandbox;
    }

    void copyPlugin(const std::filesystem::path &source, const std::filesystem::path &destination)
    {
        std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
    }
}

Test(SCENE_SYSTEM, parser_handles_nested_dotted_paths)
{
    RayTracer::Parser parser("tests/scene_system/config/nested_paths.cfg");
    const RayTracer::Object &root = std::get<RayTracer::Object>(parser._node->value);
    const RayTracer::Object &scene = std::get<RayTracer::Object>(root.at("scene")->value);

    const RayTracer::Object &camera = std::get<RayTracer::Object>(scene.at("camera")->value);
    const RayTracer::Object &settings = std::get<RayTracer::Object>(camera.at("settings")->value);
    cr_assert_float_eq(Raytracer::fromNode<float>(settings.at("exposure")), 1.5f, 1e-6);
    cr_assert_str_eq(Raytracer::fromNode<std::string>(settings.at("name")).c_str(), "primary");

    const RayTracer::Object &render = std::get<RayTracer::Object>(scene.at("render")->value);
    const RayTracer::Object &layers = std::get<RayTracer::Object>(render.at("layers")->value);
    const RayTracer::VectorObject &items = std::get<RayTracer::VectorObject>(layers.at("items")->value);
    cr_assert_eq(items.size(), 2u);
    cr_assert_eq(std::get<int>(std::get<RayTracer::ScalarValue>(items[0].at("id")->value)), 1);
    cr_assert_str_eq(std::get<std::string>(std::get<RayTracer::ScalarValue>(items[0].at("label")->value)).c_str(), "first");
    cr_assert_eq(std::get<int>(std::get<RayTracer::ScalarValue>(items[1].at("id")->value)), 2);
    cr_assert_str_eq(std::get<std::string>(std::get<RayTracer::ScalarValue>(items[1].at("label")->value)).c_str(), "second");
}

Test(SCENE_SYSTEM, parser_captures_nested_and_scalar_types)
{
    RayTracer::Parser parser("tests/scene_system/config/scene_coverage.cfg");
    const RayTracer::Object &root = std::get<RayTracer::Object>(parser._node->value);

    const RayTracer::Object &debug = std::get<RayTracer::Object>(root.at("debug")->value);
    cr_assert(std::get<bool>(std::get<RayTracer::ScalarValue>(Raytracer::Detail::field(debug, "enabled")->value)));
    cr_assert_eq(std::get<long long>(std::get<RayTracer::ScalarValue>(Raytracer::Detail::field(debug, "buildId")->value)), 4294967296LL);
    cr_assert_str_eq(std::get<std::string>(std::get<RayTracer::ScalarValue>(Raytracer::Detail::field(debug, "label")->value)).c_str(), "coverage");
    cr_assert_eq(Raytracer::fromNode<int>(Raytracer::Detail::field(debug, "sampleCount")), 3);

    const RayTracer::VectorObject &entries = std::get<RayTracer::VectorObject>(Raytracer::Detail::field(debug, "entries")->value);
    cr_assert_eq(entries.size(), 2u);
    cr_assert_eq(std::get<int>(std::get<RayTracer::ScalarValue>(entries[0].at("id")->value)), 1);
    cr_assert_str_eq(std::get<std::string>(std::get<RayTracer::ScalarValue>(entries[0].at("label")->value)).c_str(), "first");
    cr_assert_eq(std::get<int>(std::get<RayTracer::ScalarValue>(entries[1].at("id")->value)), 2);
    cr_assert_str_eq(std::get<std::string>(std::get<RayTracer::ScalarValue>(entries[1].at("label")->value)).c_str(), "second");

    const RayTracer::Object &camera = std::get<RayTracer::Object>(root.at("camera")->value);
    const std::pair<int, int> resolution = Raytracer::fromNode<std::pair<int, int>>(Raytracer::Detail::field(camera, "resolution"));
    cr_assert_eq(resolution.first, 640);
    cr_assert_eq(resolution.second, 360);

    const Geometry::Point3D position = Raytracer::fromNode<Geometry::Point3D>(Raytracer::Detail::field(camera, "position"));
    cr_assert_float_eq(position.x, 0.0f, 1e-6);
    cr_assert_float_eq(position.y, -60.0f, 1e-6);
    cr_assert_float_eq(position.z, 10.0f, 1e-6);

    const Geometry::Vector3D rotation = Raytracer::fromNode<Geometry::Vector3D>(Raytracer::Detail::field(camera, "rotation"));
    cr_assert_float_eq(rotation.x, 95.29f, 1e-4);
    cr_assert_float_eq(rotation.y, 0.0f, 1e-6);
    cr_assert_float_eq(rotation.z, 0.0f, 1e-6);

    cr_assert_float_eq(Raytracer::fromNode<float>(Raytracer::Detail::field(camera, "fieldOfView")), 100.0f, 1e-6);
}

Test(SCENE_SYSTEM, plugin_manager_loads_core_plugin)
{
    RayTracer::PluginManager manager("plugins");
    cr_assert(manager.getInitializer("camera"));
    cr_assert(manager.getInitializer("planes"));
    cr_assert(manager.getInitializer("point"));
    cr_assert(manager.getInitializer("ambient"));
}

Test(SCENE_SYSTEM, plugin_manager_skips_invalid_and_duplicate_plugins)
{
    const std::filesystem::path sandbox = makePluginSandbox();
    copyPlugin("plugins/CorePlugin.so", sandbox / "CorePlugin.so");
    copyPlugin("plugins/CorePlugin.so", sandbox / "CorePluginDuplicate.so");
    std::ofstream(sandbox / "broken.so").close();

    RayTracer::PluginManager manager(sandbox.string());
    cr_assert(manager.getInitializer("camera"));
    cr_assert(!manager.getInitializer("does_not_exist"));

    std::filesystem::remove_all(sandbox);
}

Test(SCENE_SYSTEM, scene_update_camera_rejects_non_string_type)
{
    RayTracer::Scene scene;
    RayTracer::PluginManager manager("plugins");
    RayTracer::NodePtr camera = std::make_unique<RayTracer::Node>();
    RayTracer::Object cameraObject;
    auto typeNode = std::make_unique<RayTracer::Node>();
    typeNode->value = RayTracer::ScalarValue{1};
    cameraObject["type"] = std::move(typeNode);
    camera->value = std::move(cameraObject);

    cr_assert_throw(scene.updateCamera(camera, manager), RayTracer::Exception);
}

Test(SCENE_SYSTEM, scene_update_camera_rejects_missing_initializer)
{
    const std::filesystem::path sandbox = makePluginSandbox();
    copyPlugin("plugins/BVHCamPlugin.so", sandbox / "BVHCamPlugin.so");

    RayTracer::PluginManager manager(sandbox.string());
    RayTracer::Scene scene;
    RayTracer::NodePtr camera = std::make_unique<RayTracer::Node>();
    RayTracer::Object cameraObject;
    auto typeNode = std::make_unique<RayTracer::Node>();
    typeNode->value = RayTracer::ScalarValue{std::string("camera")};
    cameraObject["type"] = std::move(typeNode);
    camera->value = std::move(cameraObject);

    cr_assert_throw(scene.updateCamera(camera, manager), RayTracer::Exception);

    std::filesystem::remove_all(sandbox);
}

Test(SCENE_SYSTEM, scene_loader_builds_scene)
{
    RayTracer::PluginManager manager("plugins");
    RayTracer::SceneLoader loader(manager, "tests/scene_system/config/scene_coverage.cfg");
    std::unique_ptr<RayTracer::Scene> scene = loader.loadScene();

    cr_assert_not_null(scene.get());
    cr_assert(scene->getRenderer() != nullptr);
    cr_assert_eq(scene->getPrimitives().size(), 1u);
    cr_assert_eq(scene->getLights().size(), 3u);

    scene->prepareAccelerationStructure();
    cr_assert(scene->getBVH().has_value());

    RayTracer::Scene copiedScene(*scene->getRenderer());
    cr_assert(copiedScene.getRenderer() != nullptr);
    cr_assert(!copiedScene.getBVH().has_value());

    const Geometry::Ray ray(Geometry::Point3D(0.0, 0.0, 10.0), Geometry::Vector3D(0.0, 0.0, -1.0));
    std::optional<Geometry::HitRecord> hit = scene->hit(ray);
    cr_assert(hit.has_value());
    cr_assert_float_eq(hit->rayDistance, 20.0f, 1e-3);
}

Test(SCENE_SYSTEM, scene_loader_rejects_missing_primitive_initializer)
{
    RayTracer::PluginManager manager("plugins");
    RayTracer::SceneLoader loader(manager, "tests/scene_system/config/missing_primitive.cfg");

    cr_assert_throw(loader.loadScene(), RayTracer::Exception);
}

Test(SCENE_SYSTEM, scene_loader_rejects_missing_light_initializer)
{
    RayTracer::PluginManager manager("plugins");
    RayTracer::SceneLoader loader(manager, "tests/scene_system/config/missing_light.cfg");

    cr_assert_throw(loader.loadScene(), RayTracer::Exception);
}

Test(SCENE_SYSTEM, plugin_manager_missing_directory_throws)
{
    cr_assert_throw(RayTracer::PluginManager("/definitely/not/a/real/plugin/path"), RayTracer::Exception);
}
