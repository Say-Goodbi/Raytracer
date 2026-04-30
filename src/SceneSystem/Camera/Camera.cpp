#include "Camera.hpp"

namespace RayTracer
{
    Camera::Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov)
    : _position(pos), _direction(direction), _fov(fov)
    {
    }

    int Camera::getHeight() const
    {
        return this->_screen.height.length();
    }

    int Camera::getWidth() const
    {
        return this->_screen.width.length();
    }

    Geometry::Ray Camera::generateRay(double u, double v)
    {
        Geometry::Point3D screenPoint = this->_screen.pointAt(u, v);
        Geometry::Vector3D rayDirection = Geometry::Vector3D(
            screenPoint.x - this->_position.x,
            screenPoint.y - this->_position.y,
            screenPoint.z - this->_position.z).normalize();
        return Geometry::Ray(this->_position, rayDirection);
    }

} // namespace RayTracer
