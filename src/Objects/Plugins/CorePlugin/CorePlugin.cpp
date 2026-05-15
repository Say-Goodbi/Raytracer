#include "Interfaces/SceneWriter/SceneWriter.hpp"
#include <cmath>
#include "../../Plugin.hpp"
#include "../../../SceneSystem/SceneNodeHelpers/SceneNodeHelpers.hpp"
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
            throw RayTracer::ParsingException("Unknown material type: " + type);
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

                    const std::pair<int, int> resolution = settingsMap.find("resolution") != settingsMap.end()
                        ? Raytracer::fromNode<std::pair<int, int>>(settingsMap.at("resolution"))
                        : std::make_pair(800, 600);
                    const Geometry::Vector3D scale = settingsMap.find("scale") != settingsMap.end()
                        ? Raytracer::fromNode<Geometry::Vector3D>(settingsMap.at("scale"))
                        : Geometry::Vector3D(1.0, 1.0, 1.0);
                    const Geometry::Point3D position = settingsMap.find("position") != settingsMap.end()
                        ? Raytracer::fromNode<Geometry::Point3D>(settingsMap.at("position"))
                        : Geometry::Point3D(0.0, 0.0, 0.0);
                    const Geometry::Vector3D rotation = settingsMap.find("rotation") != settingsMap.end()
                        ? Raytracer::fromNode<Geometry::Vector3D>(settingsMap.at("rotation"))
                        : Geometry::Vector3D(0.0, 0.0, 0.0);
                    const float fieldOfView = settingsMap.find("fieldOfView") != settingsMap.end()
                        ? Raytracer::fromNode<float>(settingsMap.at("fieldOfView"))
                        : 50.0f;
                    const bool useBVH = settingsMap.find("bvh") != settingsMap.end()
                        ? Raytracer::fromNode<bool>(settingsMap.at("bvh"))
                        : true;

                    Geometry::TransformMatrix transform = Geometry::TransformMatrix::translation(
                        static_cast<float>(position.x),
                        static_cast<float>(position.y),
                        static_cast<float>(position.z)
                    );
                    transform *= Geometry::TransformMatrix::scaling(
                        static_cast<float>(scale.x),
                        static_cast<float>(scale.y),
                        static_cast<float>(scale.z)
                    );
                    // Apply rotations in explicit axis order: X (pitch), then Y (yaw), then Z (roll)
                    transform *= Geometry::TransformMatrix::rotationX(static_cast<float>(rotation.x));
                    transform *= Geometry::TransformMatrix::rotationY(static_cast<float>(rotation.y));
                    transform *= Geometry::TransformMatrix::rotationZ(static_cast<float>(rotation.z));

                    std::shared_ptr<RayTracer::ARenderer> camera = std::make_shared<RayTracer::Camera>(transform, fieldOfView, resolution.first, resolution.second, useBVH);
                    return std::static_pointer_cast<RayTracer::ARenderer>(camera);
                }
            },
            // Primitive
            {
                "planes", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("point") == settingsMap.end() || settingsMap.find("normal") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::ParsingException("Missing required parameters for Plane primitive: point, normal, color");

                    Geometry::Point3D point = Raytracer::fromNode<Geometry::Point3D>(settingsMap.at("point"));
                    Geometry::Vector3D normal = Raytracer::fromNode<Geometry::Vector3D>(settingsMap.at("normal"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));
                    std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = Raytracer::fromNode<std::string>(settingsMap.at("material"));

                    std::shared_ptr<RayTracer::APrimitive> plane = std::make_shared<RayTracer::Plane>(point, normal, getMaterialInstance(material, color));
                    return std::static_pointer_cast<RayTracer::APrimitive>(plane);
                }
            },
            {
                "spheres", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("center") == settingsMap.end() || settingsMap.find("radius") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::ParsingException("Missing required parameters for Sphere primitive: center, radius, color");

                    const float radius = Raytracer::fromNode<float>(settingsMap.at("radius"));
                    Geometry::Point3D center = Raytracer::fromNode<Geometry::Point3D>(settingsMap.at("center"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));
                    std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = Raytracer::fromNode<std::string>(settingsMap.at("material"));
                    std::shared_ptr<RayTracer::APrimitive> sphere = std::make_shared<RayTracer::Sphere>(center, radius, getMaterialInstance(material, color));
                    return std::static_pointer_cast<RayTracer::APrimitive>(sphere);
                }
            },
            {
                "cylinders", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("origin") == settingsMap.end() || settingsMap.find("axis") == settingsMap.end() || settingsMap.find("radius") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::ParsingException("Missing required parameters for Cylinder primitive: origin, axis, radius, color");

                    const float radius = Raytracer::fromNode<float>(settingsMap.at("radius"));
                    Geometry::Point3D origin = Raytracer::fromNode<Geometry::Point3D>(settingsMap.at("origin"));
                    Geometry::Vector3D axis = Raytracer::fromNode<Geometry::Vector3D>(settingsMap.at("axis"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));
                    std::string material = std::string("lambertian");
                    if (settingsMap.find("material") != settingsMap.end())
                        material = Raytracer::fromNode<std::string>(settingsMap.at("material"));
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
                        throw RayTracer::ParsingException("Missing required parameters for PointLight: position, intensity, color");

                    const float intensity = Raytracer::fromNode<float>(settingsMap.at("intensity"));
                    Geometry::Point3D position = Raytracer::fromNode<Geometry::Point3D>(settingsMap.at("position"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));

                    std::shared_ptr<RayTracer::ILight> pointLight = std::make_shared<RayTracer::PointLight>(position, intensity, color);
                    return std::static_pointer_cast<RayTracer::ILight>(pointLight);
                }
            },
            {
                "directional", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("position") == settingsMap.end() || settingsMap.find("intensity") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::ParsingException("Missing required parameters for DirectionalLight: position, intensity, color");

                    const float intensity = Raytracer::fromNode<float>(settingsMap.at("intensity"));
                    Geometry::Vector3D position = Raytracer::fromNode<Geometry::Vector3D>(settingsMap.at("position"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));

                    std::shared_ptr<RayTracer::ILight> directionalLight = std::make_shared<RayTracer::DirectionalLight>(position, intensity, color);
                    return std::static_pointer_cast<RayTracer::ILight>(directionalLight);
                }
            },
            {
                "ambient", [](RayTracer::NodePtr node) -> RayTracer::Component
                {
                    const RayTracer::Object &settingsMap = std::get<RayTracer::Object>(node->value);

                    if (settingsMap.find("multiplier") == settingsMap.end() || settingsMap.find("color") == settingsMap.end())
                        throw RayTracer::ParsingException("Missing required parameters for AmbientLight: intensity, color");

                    const float ambient = Raytracer::fromNode<float>(settingsMap.at("multiplier"));
                    RayTracer::Color color = Raytracer::fromNode<RayTracer::Color>(settingsMap.at("color"));

                    std::shared_ptr<RayTracer::ILight> ambientLight = std::make_shared<RayTracer::AmbientLight>(ambient, color);
                    return std::static_pointer_cast<RayTracer::ILight>(ambientLight);
                }
            }
        };
    }
};
