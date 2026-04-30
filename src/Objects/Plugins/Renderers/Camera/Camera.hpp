#pragma once
#include "../../Objects/Abstracts/ARenderer/ARenderer.hpp"
#include "../../Geometry/Point3D/Point3D.hpp"
#include "../../Geometry/Vector3D/Vector3D.hpp"
#include "../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../Geometry/Ray/Ray.hpp"
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include <optional>

namespace RayTracer
{
    class Camera : public ARenderer
    {
    private:
        Geometry::Point3D _position;
        Geometry::Vector3D _direction;
        Geometry::Rectangle3D _screen;
        float _fov;
        
        Color castRay(Geometry::Ray& ray, Scene& scene);
        bool computeShadow(Geometry::HitRecord& hit, Scene& scene);
    
    protected:
    public:
        Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width = 800, int height = 600) 
        : ARenderer(width, height), _position(pos), _direction(direction), _fov(fov) {};

        ~Camera() = default;
        
        Geometry::Ray generateRay(double u, double v);

        std::vector<std::vector<Color>> render(Scene& scene) override;
    };
} // namespace Raytracer
