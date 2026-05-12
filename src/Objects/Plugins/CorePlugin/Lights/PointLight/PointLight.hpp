#pragma once
#include "../../../../../Objects/Abstracts/ILight.hpp"

namespace RayTracer
{
    /**
     * @class PointLight
     * @brief A punctual light source emitting equally in all directions.
     *
     * Performs shadow testing by casting a ray from the hit point toward
     * the light position. If occluded, contribution is attenuated.
     */
    class PointLight : public ILight
    {
    private:
        Geometry::Point3D _position;  ///< Light position in world space
        double _intensity;             ///< Light intensity multiplier
        Color _color;                  ///< Light color

    public:
        PointLight(Geometry::Point3D position, double intensity, Color color);
        ~PointLight() = default;

        /// @brief Compute light contribution
        Color computeLight(const Geometry::HitRecord& hit, const std::vector<std::shared_ptr<APrimitive>>& primitives) const override;
    };
} // namespace RayTracer