#pragma once
#include "../../../../../Objects/Abstracts/ILight.hpp"

namespace RayTracer
{
    /// Directional light — no position, only a direction (simulates sunlight).
    class DirectionalLight : public ILight
    {
    private:
        Geometry::Vector3D _direction;  ///< Direction the light travels
        double _intensity;               ///< Intensity multiplier
        Color _color;                    ///< Light color

    public:
        DirectionalLight(Geometry::Vector3D direction, double intensity, Color color);
        ~DirectionalLight() = default;

        /// @brief Compute light contribution, shadow ray cast in opposite direction
        Color computeLight( const Geometry::HitRecord& hit, const std::vector<std::shared_ptr<APrimitive>>& primitives) const override;
    };
} // namespace RayTracer