#pragma once
#include "../../../Objects/Abstracts/ARenderer/ARenderer.hpp"
#include "../../../Geometry/Point3D/Point3D.hpp"
#include "../../../Geometry/Vector3D/Vector3D.hpp"
#include "../../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../../Geometry/Ray/Ray.hpp"
#include "../../../Geometry/HitRecord/HitRecord.hpp"
#include <optional>

namespace RayTracer
{
    /**
     * @class Camera
     * @brief Concrete renderer plugin that implements camera-based ray casting.
     *
     * The camera converts normalized screen coordinates into rays, traces those
     * rays against scene primitives, applies basic shadowing, and returns the
     * resulting framebuffer.
     */
    class Camera : public ARenderer
    {
    private:
        /// Camera world-space position.
        Geometry::Point3D _position;
        /// Camera forward direction.
        Geometry::Vector3D _direction;
        /// Camera projection/screen rectangle in world space.
        Geometry::Rectangle3D _screen;
        /// Field of view in degrees.
        float _fov;
        /// Maximum recursion depth for ray bounces
        int _maxDepth = 8;
        /// 2 means 4 rays per pixel, 3 means 9 rays per pixel, etc.
        int _sqrtSamples = 2;


        /**
         * @brief Trace a single ray in the scene and compute the resulting color.
         * @param ray Ray to cast.
         * @param scene Scene containing primitives and lights.
         * @param depth Recursion depth for reflection/refraction.
         * @return Final shaded color for this ray.
         */
        Color Camera::castRay(const Geometry::Ray& ray, Scene& scene, int depth);
    
    protected:
    public:
        /**
         * @brief Construct a camera renderer.
         * @param pos Camera position.
         * @param direction Camera forward direction.
         * @param fov Field of view in degrees.
         * @param width Output framebuffer width.
         * @param height Output framebuffer height.
         */
        Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width = 800, int height = 600) 
        : ARenderer(width, height), _position(pos), _direction(direction), _fov(fov) {};

        ~Camera() = default;

        /**
         * @brief Generate a world-space ray from normalized screen coordinates.
         * @param u Horizontal coordinate in [0, 1].
         * @param v Vertical coordinate in [0, 1].
         * @return Ray emitted from the camera through the screen point.
         */
        Geometry::Ray generateRay(double u, double v);

        /**
         * @brief Render a full scene into a 2D framebuffer.
         * @param scene Scene to render.
         * @return 2D framebuffer of colors (height x width).
         */
        std::vector<std::vector<Color>> render(Scene& scene) override;
    };
} // namespace RayTracer
