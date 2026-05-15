#include "Interfaces/SceneWriter/SceneWriter.hpp"
#include <cmath>
#include "../../Plugin.hpp"
#include "../../../Geometry/Point3D/Point3D.hpp"
#include "../../../Geometry/Vector3D/Vector3D.hpp"
#include "../../../Utils/Color.hpp"
#include "Materials/Lambertian/Lambertian.hpp"
#include "Materials/Phong/Phong.hpp"
#include "Materials/Glass/Glass.hpp"
#include "Primitives/Plane/Plane.hpp"
#include "Primitives/Sphere/Sphere.hpp"
#include "Primitives/Cylinder/Cylinder.hpp"
#include "Lights/PointLight/PointLight.hpp"
#include "Lights/DirectionalLight/DirectionalLight.hpp"
#include "Lights/AmbientLight/AmbientLight.hpp"
#include "Renderers/Camera/Camera.hpp"


extern "C"
{
    std::map<std::string, std::function<std::shared_ptr<RayTracer::IMaterial>(const RayTracer::Color &)>> _materials = {
        {"lambertian", [](const RayTracer::Color &color) -> std::shared_ptr<RayTracer::IMaterial>
        {
            return std::static_pointer_cast<RayTracer::IMaterial>(std::make_shared<RayTracer::Lambertian>(color));
        }},
        {"phong", [](const RayTracer::Color &color) -> std::shared_ptr<RayTracer::IMaterial>
        {
            return std::static_pointer_cast<RayTracer::IMaterial>(std::make_shared<RayTracer::Phong>(color));
        }},
        {"glass", [](const RayTracer::Color &color) -> std::shared_ptr<RayTracer::IMaterial>
        {
            return std::static_pointer_cast<RayTracer::IMaterial>(std::make_shared<RayTracer::Glass>(color));
        }}
    };

    std::shared_ptr<RayTracer::IMaterial> getMaterialInstance(const std::string &type, const RayTracer::Color &color)
    {
        std::map<std::string, std::function<std::shared_ptr<RayTracer::IMaterial>(const RayTracer::Color &)>>::const_iterator it = _materials.find(type);
    
        if (it == _materials.end())
            throw RayTracer::Exception("Unknown material type: " + type);
        return it->second(color);
    }

    std::string getName() { return "CorePlugin"; };

    std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>> getInitializers()
    {
        return {
            {
                "camera", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    

                    if (settingsMap.find("resolution") == settingsMap.end() || settingsMap.find("position") == settingsMap.end() || settingsMap.find("rotation") == settingsMap.end() || settingsMap.find("fieldOfView") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for Camera: resolution, position, rotation, fieldOfView");

                    const RayTracer::Object &ref_resolution = std::get<RayTracer::Object>(settingsMap.at("resolution")->value);
                    const RayTracer::Object &ref_position = std::get<RayTracer::Object>(settingsMap.at("position")->value);
                    const RayTracer::Object &ref_rotation = std::get<RayTracer::Object>(settingsMap.at("rotation")->value);
                    const float fieldOfView = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("fieldOfView")->value));
                    int width = std::get<int>(std::get<RayTracer::ScalarValue>(ref_resolution.at("width")->value));
                    int height = std::get<int>(std::get<RayTracer::ScalarValue>(ref_resolution.at("height")->value));
                    Geometry::Point3D position = Geometry::Point3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("z")->value)));
                    Geometry::Vector3D rotation = Geometry::Vector3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_rotation.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_rotation.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_rotation.at("z")->value)));

                    // Convert Euler rotation to a direction vector 
                    float rx = rotation.x * (float)(M_PI / 180.0);
                    float ry = rotation.y * (float)(M_PI / 180.0);
                    Geometry::Vector3D forward(
                        -std::sin(ry) * std::cos(rx),
                         std::cos(ry) * std::cos(rx),
                         std::sin(rx));
                    std::shared_ptr<RayTracer::ARenderer> camera = std::make_shared<RayTracer::Camera>(position, forward, fieldOfView, width, height);
                    return std::static_pointer_cast<RayTracer::ARenderer>(camera);
                }
            },
            // Primitive
            {
                "planes", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("point") == settingsMap.end() || settingsMap.find("normal") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for Plane primitive: point, normal, color");

                    const RayTracer::Object &ref_point = std::get<RayTracer::Object>(settingsMap.at("point")->value);
                    const RayTracer::Object &ref_normal = std::get<RayTracer::Object>(settingsMap.at("normal")->value);
                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    Geometry::Point3D point = Geometry::Point3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_point.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_point.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_point.at("z")->value)));
                    Geometry::Vector3D normal = Geometry::Vector3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_normal.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_normal.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_normal.at("z")->value)));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);
                        std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("material")->value));

                    std::shared_ptr<RayTracer::APrimitive> plane = std::make_shared<RayTracer::Plane>(point, normal, getMaterialInstance(material, color));
                    return std::static_pointer_cast<RayTracer::APrimitive>(plane);
                }
            },
            {
                "spheres", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("center") == settingsMap.end() || settingsMap.find("radius") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for Sphere primitive: center, radius, color");

                    const RayTracer::Object &ref_center = std::get<RayTracer::Object>(settingsMap.at("center")->value);
                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    const float radius = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("radius")->value));
                    Geometry::Point3D center = Geometry::Point3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_center.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_center.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_center.at("z")->value)));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);
                    std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("material")->value));
                    std::shared_ptr<RayTracer::APrimitive> sphere = std::make_shared<RayTracer::Sphere>(center, radius, getMaterialInstance(material, color));
                    return std::static_pointer_cast<RayTracer::APrimitive>(sphere);
                }
            },
            {
                "cylinders", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("origin") == settingsMap.end() || settingsMap.find("axis") == settingsMap.end() || settingsMap.find("radius") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for Cylinder primitive: origin, axis, radius, color");

                    const RayTracer::Object &ref_origin = std::get<RayTracer::Object>(settingsMap.at("origin")->value);
                    const RayTracer::Object &ref_axis = std::get<RayTracer::Object>(settingsMap.at("axis")->value);
                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    const float radius = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("radius")->value));
                    Geometry::Point3D origin = Geometry::Point3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_origin.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_origin.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_origin.at("z")->value)));
                    Geometry::Vector3D axis = Geometry::Vector3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_axis.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_axis.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_axis.at("z")->value)));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);
                    std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("material")->value));
                    std::shared_ptr<RayTracer::APrimitive> cylinder = std::make_shared<RayTracer::Cylinder>(origin, axis, radius, getMaterialInstance(material, color));
                    return std::static_pointer_cast<RayTracer::APrimitive>(cylinder);
                }
            },
            // Light
            {
                "point", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("position") == settingsMap.end() || settingsMap.find("intensity") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for PointLight: position, intensity, color");

                    const RayTracer::Object &ref_position = std::get<RayTracer::Object>(settingsMap.at("position")->value);
                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    const float intensity = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("intensity")->value));
                    Geometry::Point3D position = Geometry::Point3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("z")->value)));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);

                    std::shared_ptr<RayTracer::ILight> pointLight = std::make_shared<RayTracer::PointLight>(position, intensity, color);
                    return std::static_pointer_cast<RayTracer::ILight>(pointLight);
                }
            },
            {
                "directional", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("position") == settingsMap.end() || settingsMap.find("intensity") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for DirectionalLight: position, intensity, color");

                    const RayTracer::Object &ref_position = std::get<RayTracer::Object>(settingsMap.at("position")->value);
                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    const float intensity = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("intensity")->value));
                    Geometry::Vector3D position = Geometry::Vector3D(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("x")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("y")->value)),
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_position.at("z")->value)));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);

                    std::shared_ptr<RayTracer::ILight> directionalLight = std::make_shared<RayTracer::DirectionalLight>(position, intensity, color);
                    return std::static_pointer_cast<RayTracer::ILight>(directionalLight);
                }
            },
            {
                "ambient", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("multiplier") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::Exception("Missing required parameters for AmbientLight: intensity, color");

                    const RayTracer::Object &ref_color = std::get<RayTracer::Object>(settingsMap.at("color")->value);
                    const float ambient = std::get<float>(std::get<RayTracer::ScalarValue>(settingsMap.at("multiplier")->value));
                    RayTracer::Color color = RayTracer::Color(
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("r")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("g")->value)) / 255.0,
                        std::get<float>(std::get<RayTracer::ScalarValue>(ref_color.at("b")->value)) / 255.0);

                    std::shared_ptr<RayTracer::ILight> ambientLight = std::make_shared<RayTracer::AmbientLight>(ambient, color);
                    return std::static_pointer_cast<RayTracer::ILight>(ambientLight);
                }
            }
        };
    }
};
