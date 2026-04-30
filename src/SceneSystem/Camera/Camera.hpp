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
    /// Concrete camera implementation inheriting from ARenderer.
    /// Holds camera position, direction, and FOV; generates rays and casts them into the scene.
    class Camera : public ARenderer
    {
    private:
        Geometry::Point3D _position;
        Geometry::Vector3D _direction;
        Geometry::Rectangle3D _screen;
        float _fov;
        
        /// Cast a ray into the scene and compute intersection color.
        Color castRay(Geometry::Ray& ray, Scene& scene);
        /// Compute shadow at a hit point from all lights.
        bool computeShadow(Geometry::HitRecord& hit, Scene& scene);
    
    protected:
    public:
        /// Constructor.
        /// @param pos Camera position
        /// @param direction View direction
        /// @param fov Field of view in degrees
        /// @param width Output width in pixels (default 800)
        /// @param height Output height in pixels (default 600)
        Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width = 800, int height = 600) 
        : ARenderer(width, height), _position(pos), _direction(direction), _fov(fov) {};

        ~Camera() = default;
        
        /// Generate a ray for normalized screen coordinates (u, v).
        /// @param u Normalized x coordinate [0, 1]
        /// @param v Normalized y coordinate [0, 1]
        /// @return Ray from camera through screen point
        Geometry::Ray generateRay(double u, double v);

        /// Render the scene by casting rays and returning a framebuffer.
        /// @param scene The scene to render
        /// @return 2D framebuffer (height x width)
        std::vector<std::vector<Color>> render(Scene& scene) override;
    };
} // namespace Raytracer
