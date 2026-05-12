#include "Interfaces/SceneWriter/SceneWriter.hpp"
#include "../../Plugin.hpp"
#include "../../../Geometry/Point3D/Point3D.hpp"
#include "../../../Geometry/Vector3D/Vector3D.hpp"
#include "../../../Utils/Color.hpp"
#include "Materials/Lambertian/Lambertian.hpp"
#include "Primitives/Plane/Plane.hpp"
#include "Primitives/Sphere/Sphere.hpp"
#include "Lights/PointLight/PointLight.hpp"
#include "Lights/DirectionalLight/DirectionalLight.hpp"
#include "Lights/AmbientLight/AmbientLight.hpp"
#include "Renderers/Camera/Camera.hpp"

extern "C"
{
    std::string getName() { return "CorePlugin"; };

    std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>> getInitializers()
    {
        return {
            // @todo Fix :
            // {"SceneWriter", [](RayTracer::NodePtr node) -> RayTracer::Component
            //  {
            //      RayTracer::Object settingsMap = std::get<RayTracer::Object>(node->value);
            //      auto rendererPtr = std::get<std::shared_ptr<RayTracer::ARenderer>>(std::get<RayTracer::ScalarValue>(settingsMap.at("renderer")));
            //      auto outputFile = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("outputFile")));
            //      auto sw = std::make_shared<RayTracer::SceneWriter>(rendererPtr.get(), outputFile);
            //      return std::static_pointer_cast<RayTracer::AInterface>(sw);
            //  }}
            // {
            //     "Camera", [](RayTracer::NodePtr node) -> RayTracer::Component
            //     {
            //         return ;
            //     }
            // },
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

                    std::shared_ptr<RayTracer::ARenderer> camera = std::make_shared<RayTracer::Camera>(position, rotation, fieldOfView, width, height);
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
                    std::shared_ptr<RayTracer::Lambertian> material = std::make_shared<RayTracer::Lambertian>(color);

                    std::shared_ptr<RayTracer::APrimitive> plane = std::make_shared<RayTracer::Plane>(point, normal, material.get());
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
                    std::shared_ptr<RayTracer::Lambertian> material = std::make_shared<RayTracer::Lambertian>(color);

                    std::shared_ptr<RayTracer::APrimitive> sphere = std::make_shared<RayTracer::Sphere>(center, radius, material.get());
                    return std::static_pointer_cast<RayTracer::APrimitive>(sphere);
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
}
