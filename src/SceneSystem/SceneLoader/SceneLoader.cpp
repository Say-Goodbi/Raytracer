#include "SceneLoader.hpp"

RayTracer::SceneLoader::SceneLoader(const std::string &filename) : _pluginManager("plugins"), _parser(filename) {}

RayTracer::Scene RayTracer::SceneLoader::createCamera(RayTracer::Object &cameraSettings)
{
    auto initializer = _pluginManager.getInitializer("camera");
    if (!initializer)
        throw RayTracerException("Camera initializer not found in plugins.");
    return std::get<Camera>(initializer(cameraSettings));
}

std::unique_ptr<APrimitive> RayTracer::SceneLoader::createPrimitive(RayTracer::Object &primitiveSettings)
{
    const auto &primitives = std::get<RayTracer::Object>(_parser._node->value).at("primitives");

    if (std::get<RayTracer::Object>(primitives->value).find("spheres") != std::get<RayTracer::Object>(primitives->value).end())
    { // TO DO
        const RayTracer::Object &spheres = std::get<RayTracer::Object>(primitives->value).at("spheres");
        for (const RayTracer::Object &sphereNode : std::get<RayTracer::Object>(spheres->value))
            for (const auto &[key, nodePtr] : sphereNode)
            {
                RayTracer::Sphere sphere = new RayTracer::Sphere();
                _scene.addPrimitive(sphere);
            }
    }
    if (std::get<RayTracer::Object>(primitives->value).find("planes") != std::get<RayTracer::Object>(primitives->value).end())
    {
        const RayTracer::Object &planes = std::get<RayTracer::Object>(primitives->value).at("planes");
        for (const RayTracer::Object &planesNode : std::get<RayTracer::Object>(planes->value))
        {
            Geometry::Point3D point = {0.0, 0.0, 0.0};
            Geometry::Vector3D normal = {0.0, 0.0, 0.0};
            RayTracer::Color color = {0.0, 0.0, 0.0};
            for (const auto &[key, nodePtr] : planesNode)
            {
                if (key == "point")
                    point = {
                        .x = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("x")->value)),
                        .y = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("y")->value)),
                        .z = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("z")->value))};
                else if (key == "normal")
                    normal = {
                        .x = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("x")->value)),
                        .y = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("y")->value)),
                        .z = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("z")->value))};
                else if (key == "color")
                    color = {
                        .r = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("r")->value)),
                        .g = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("g")->value)),
                        .b = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("b")->value))};
            }
            RayTracer::Plane plane = new RayTracer::Plane(point, normal, color);
            _scene.addPrimitive(plane);
        }
    }
    return primitive;
}

std::unique_ptr<ILight> RayTracer::SceneLoader::createLight(RayTracer::Object &lightSettings)
{
    const RayTracer::Object &lights = std::get<RayTracer::Object>(_parser._node->value).at("lights");

    if (std::get<RayTracer::Object>(lights->value).find("point") != std::get<RayTracer::Object>(lights->value).end())
    {
        const RayTracer::Object &point = std::get<RayTracer::Object>(lights->value).at("point");
        for (const RayTracer::Object &sphereNode : std::get<RayTracer::Object>(point->value))
        {
            Geometry::Point3D _position = {0.0, 0.0, 0.0};
            double _intensity = 0.0;
            RayTracer::Color _color = {0.0, 0.0, 0.0};
            for (const auto &[key, nodePtr] : sphereNode)
            {
                if (key == "position")
                    _position = {
                        .x = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("x")->value)),
                        .y = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("y")->value)),
                        .z = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("z")->value))};
                else if (key == "intensity")
                    _intensity = std::get<double>(std::get<RayTracer::ScalarValue>(nodePtr->value));
                else if (key == "color")
                    _color = {
                        .r = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("r")->value)),
                        .g = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("g")->value)),
                        .b = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("b")->value))};
            }
            RayTracer::PointLight pointLight = new RayTracer::PointLight(_position, _intensity, _color);
            _scene.addLight(pointLight);
        }
    }
    if (std::get<RayTracer::Object>(lights->value).find("directional") != std::get<RayTracer::Object>(lights->value).end())
    {
        const RayTracer::Object &directional = std::get<RayTracer::Object>(lights->value).at("directional");
        for (const RayTracer::Object &sphereNode : std::get<RayTracer::Object>(directional->value))
        {
            Geometry::Vector3D _direction = {0.0, 0.0, 0.0};
            double _intensity = 0.0;
            RayTracer::Color _color = {0.0, 0.0, 0.0};
            RayTracer::DirectionalLight directionalLight = new RayTracer::DirectionalLight();
            for (const auto &[key, nodePtr] : sphereNode)
            {
                if (key == "direction")
                    _direction = {
                        .x = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("x")->value)),
                        .y = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("y")->value)),
                        .z = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("z")->value))};
                else if (key == "intensity")
                    _intensity = std::get<double>(std::get<RayTracer::ScalarValue>(nodePtr->value));
                else if (key == "color")
                    _color = {
                        .r = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("r")->value)),
                        .g = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("g")->value)),
                        .b = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(nodePtr->value).at("b")->value))};
            }
            _scene.addLight(directionalLight);
        }
    }
    if (std::get<RayTracer::Object>(lights->value).find("ambient") != std::get<RayTracer::Object>(lights->value).end())
    {
        const double &ambient = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(lights->value).at("ambient")->value));
        RayTracer::Color _color = {0.0, 0.0, 0.0};

        RayTracer::AmbientLight ambientLight = new RayTracer::AmbientLight();
        const auto &color = std::get<RayTracer::Object>(lights->value).at("color");
        _color = {
            .r = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(color->value).at("r")->value)),
            .g = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(color->value).at("g")->value)),
            .b = std::get<double>(std::get<RayTracer::ScalarValue>(std::get<RayTracer::Object>(color->value).at("b")->value))};
        _scene.addLight(ambientLight);
    }
    return primitive;
}

RayTracer::Scene *RayTracer::SceneLoader::loadScene(void)
{
    RayTracer::Scene *scene = nullptr;

    for (const RayTracer::Object &[key, nodePtr] : std::get<RayTracer::Object>(parser._node->value))
    {
        if (key == "camera" && nodePtr->value.index() == 1)
            scene = new RayTracer::Scene(createCamera(std::get<RayTracer::Object>(nodePtr->value)));
        else if (key == "primitives" && nodePtr->value.index() == 1)
            scene->addPrimitive(createPrimitive(std::get<RayTracer::Object>(nodePtr->value)));
        else if (key == "lights" && nodePtr->value.index() == 1)
            scene->addLight(createLight(std::get<RayTracer::Object>(nodePtr->value)));
    }
    return scene;
}
