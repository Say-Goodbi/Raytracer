#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "ILight.hpp"
#include <vector>

namespace RayTracer
{
    /**
     * @interface IMaterial
     * @brief Interface for physically-based surface materials.
     *
     * Defines the three quantities the path tracer needs to evaluate the
     * rendering equation at a surface hit — plus an emission term for
     * light-emitting surfaces:
     *
     * @code
     *   L ≈ emission() + evaluate(ωi, ωo) * Li * cosθ / pdf(ωi)
     * @endcode
     *
     * Because the renderer is deterministic, `sample()` must return a **fixed**
     * direction (e.g. perfect mirror direction, cosine-lobe centroid) rather
     * than drawing from a random distribution.  `pdf()` must be consistent with
     * that choice so the Monte Carlo weight `cosθ / pdf` remains correct.
     */
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        /**
         * @brief Evaluate the BRDF for a given pair of directions.
         *
         * Returns the fraction of light arriving from @p lightDir that is
         * scattered toward @p viewDir at a surface with the given @p normal.
         *
         * @param normal   Unit surface normal at the hit point.
         * @param viewDir  Unit direction toward the viewer (away from surface).
         * @param lightDir Unit direction toward the light / bounce origin.
         * @return BRDF value as an RGB multiplier (component-wise).
         */
        virtual Color evaluate(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir, const Geometry::Vector3D &lightDir) const = 0;

        /**
         * @brief Return the deterministic bounce direction for this material.
         *
         * Instead of sampling randomly from the material's lobe distribution,
         * implementations return a single representative direction (e.g. the
         * perfect reflection vector, the hemisphere normal, or the lobe centroid).
         * This keeps the entire render free of random numbers.
         *
         * @param normal  Unit surface normal at the hit point.
         * @param viewDir Unit direction toward the viewer (away from surface).
         * @return Unit bounce direction in the hemisphere above @p normal.
         */
        virtual Geometry::Vector3D sample(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir) const = 0;

        /**
         * @brief Return the probability density of the direction from `sample()`.
         *
         * Must be consistent with the direction returned by `sample()` so that
         * the estimator weight `cosθ / pdf` correctly accounts for the choice
         * of bounce direction.  A value near zero indicates a degenerate or
         * impossible direction; the integrator will skip the bounce in that case.
         *
         * @param normal    Unit surface normal at the hit point.
         * @param viewDir   Unit direction toward the viewer.
         * @param sampleDir Unit direction returned by `sample()`.
         * @return PDF value ≥ 0 for @p sampleDir under this material's distribution.
         */
        virtual float pdf(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir, const Geometry::Vector3D &sampleDir) const = 0;

        /**
         * @brief Return the radiance emitted by this surface.
         *
         * Non-emissive materials should return `Color(0, 0, 0)`.
         * Light sources return their emission color here; the path tracer adds
         * it to the scattered term unconditionally.
         *
         * @return Emitted radiance as an RGB color.
         */
        virtual Color emission() const = 0;
    };
} // namespace RayTracer
