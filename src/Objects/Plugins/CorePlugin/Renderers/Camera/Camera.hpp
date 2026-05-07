#pragma once
#include "../../../../../Objects/Abstracts/ARenderer/ARenderer.hpp"
#include "../../../../../Geometry/Point3D/Point3D.hpp"
#include "../../../../../Geometry/Vector3D/Vector3D.hpp"
#include "../../../../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../../../../Geometry/Ray/Ray.hpp"
#include "../../../../../Geometry/HitRecord/HitRecord.hpp"
#include <optional>

namespace RayTracer
{
    /**
     * @class Camera
     * @brief Renderer that evaluates the rendering equation deterministically.
     *
     * This camera uses the Monte Carlo path-tracing framework (BRDF evaluation,
     * PDF weighting, recursive bounces) but removes all randomness:
     *
     * - **Bounce directions** are chosen deterministically by each material's
     *   `IMaterial::sample()` (e.g. perfect specular reflection, cosine-lobe
     *   centroid) instead of being drawn from a random distribution.
     * - **Anti-aliasing** is done with a fixed stratified sub-pixel grid
     *   (`_sqrtSamples × _sqrtSamples` evenly-spaced samples) rather than
     *   jittered or random samples.
     *
     * The result is fully reproducible: identical inputs always produce
     * identical output, with no noise and no need for a random seed.
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
        /// Maximum number of recursive ray bounces before the path is terminated.
        int _maxDepth = 8;
        /// Side length of the per-pixel stratified sample grid (total samples = _sqrtSamples²).
        int _sqrtSamples = 2;

        /**
         * @brief Evaluate the rendering equation along a ray, deterministically.
         *
         * At each surface hit the bounce direction comes from `IMaterial::sample()`,
         * which is deterministic — no random numbers are involved.
         *
         * @param ray   Incoming ray to evaluate.
         * @param scene Scene containing all primitives.
         * @param depth Remaining bounce budget; returns black when ≤ 0.
         * @return Radiance estimate along @p ray.
         */
        Color castRay(const Geometry::Ray& ray, Scene& scene, int depth);

    protected:
    public:
        /**
         * @brief Construct a camera renderer.
         * @param pos       Camera position in world space.
         * @param direction Camera forward direction (need not be unit length).
         * @param fov       Horizontal field of view in degrees.
         * @param width     Output framebuffer width in pixels (default 800).
         * @param height    Output framebuffer height in pixels (default 600).
         */
        Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width = 800, int height = 600)
        : ARenderer(width, height), _position(pos), _direction(direction), _fov(fov) {};

        ~Camera() = default;

        /**
         * @brief Build a world-space ray from normalized screen coordinates.
         * @param u Horizontal position in [0, 1] (0 = left, 1 = right).
         * @param v Vertical position in [0, 1] (0 = top, 1 = bottom).
         * @return Normalized ray from the camera through the given screen point.
         */
        Geometry::Ray generateRay(double u, double v);

        /**
         * @brief Render the scene into a 2D framebuffer.
         * @param scene Scene to render.
         * @return 2D grid of clamped colors: `frame[y][x]`, row-major, top-to-bottom.
         */
        std::vector<std::vector<Color>> render(Scene& scene) override;
    };
} // namespace RayTracer
