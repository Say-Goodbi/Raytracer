#pragma once

#include "../../Geometry/Point3D/Point3D.hpp"
#include "../../Geometry/Vector3D/Vector3D.hpp"
#include "../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../Geometry/Ray/Ray.hpp"

namespace RayTracer
{
    class Camera
    {
    private:
        Geometry::Point3D _position;
        Geometry::Vector3D _direction;
        Geometry::Rectangle3D _screen;
        float _fov;
    protected:
    public:
        Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov);
        Geometry::Ray generateRay(double u, double v);
        int getWidth() const;
        int getHeight() const;
    };
} // namespace Raytracer
