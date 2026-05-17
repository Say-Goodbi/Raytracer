#include "../../Plugin.hpp"
#include "Renderers/BVHCam.hpp"
#include "../../../SceneSystem/SceneNodeHelpers/SceneNodeHelpers.hpp"

extern "C"
{
    std::string getName() { return "BVHCamPlugin"; }

    std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>> getInitializers()
    {
        auto createCamera = [](RayTracer::NodePtr node) -> RayTracer::Component
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

            Geometry::TransformMatrix transform = Geometry::TransformMatrix::translation(
                static_cast<float>(position.x),
                static_cast<float>(position.y),
                static_cast<float>(position.z)
            );
            // Apply rotations in explicit axis order: X (pitch), then Y (yaw), then Z (roll)
            transform *= Geometry::TransformMatrix::rotationX(static_cast<float>(rotation.x));
            transform *= Geometry::TransformMatrix::rotationY(static_cast<float>(rotation.y));
            transform *= Geometry::TransformMatrix::rotationZ(static_cast<float>(rotation.z));
            transform *= Geometry::TransformMatrix::scaling(
                static_cast<float>(scale.x),
                static_cast<float>(scale.y),
                static_cast<float>(scale.z)
            );

            std::shared_ptr<RayTracer::ARenderer> camera = std::make_shared<RayTracer::BVHCam>(transform, fieldOfView, resolution.first, resolution.second);
            return std::static_pointer_cast<RayTracer::ARenderer>(camera);
        };

        return {
            {"bvh", createCamera}
        };
    }
}