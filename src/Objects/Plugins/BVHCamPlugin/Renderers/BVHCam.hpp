/**
 * @file src/Objects/Plugins/CorePlugin/Renderers/Camera/Camera.hpp
 * @brief Declaration for the camera-based renderer used by the raytracer.
 *
 * Declares `RayTracer::BVHCam`, an `ARenderer` subclass that constructs rays
 * from a camera transform and evaluates the rendering equation deterministically
 * using the scene. See @ref RayTracer::ARenderer and @ref Geometry::TransformMatrix.
 */
#pragma once
#include "../../../../Objects/Abstracts/ARenderer/ARenderer.hpp"
#include "../../../../Geometry/Point3D/Point3D.hpp"
#include "../../../../Geometry/Vector3D/Vector3D.hpp"
#include "../../../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../../../Geometry/Ray/Ray.hpp"
#include "../../../../Geometry/HitRecord/HitRecord.hpp"
#include "../../../../Geometry/TransformMatrix/TransformMatrix.hpp"
#include <optional>
#include <vector>

namespace RayTracer
{
    /**
    * @class BVHCam
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
    class BVHCam : public ARenderer
    {
    private:
        /// Camera transform (world-space): encodes position + orientation.
        Geometry::TransformMatrix _transform;
        /// Camera projection/screen rectangle in world space.
        Geometry::Rectangle3D _screen;
        /// Field of view in degrees.
        float _fov;
        /// Side length of the per-pixel stratified sample grid (total samples = _sqrtSamples²).
        int _sqrtSamples = 1;

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
        BVHCam(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width = 800, int height = 600);

        /**
         * @brief Construct a camera from a 4x4 affine transform matrix.
         * @param transform Camera-to-world transform (columns = right, up, forward, translation).
         */
        BVHCam(Geometry::TransformMatrix transform, float fov, int width = 800, int height = 600);

    private:
        /// Rebuild the _screen rectangle from the current _transform and _fov.
        void buildScreenFromTransform();

    public:
        ~BVHCam() = default;

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
        std::vector<std::vector<Color>> render(Scene& scene, std::vector<std::vector<Color>>& framebuffer) override;
    };
} // namespace RayTracer
