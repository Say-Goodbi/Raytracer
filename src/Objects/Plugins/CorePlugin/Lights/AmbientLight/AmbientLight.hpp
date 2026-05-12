#pragma once
#include "../../../../../Objects/Abstracts/ILight.hpp"

namespace RayTracer
{
    class AmbientLight : public ILight
    {
    private:
        float _intensity;
        Color _color;

    public:
        AmbientLight(float intensity, Color color);
        ~AmbientLight() = default;

        /// @brief Ambient light never casts shadows, just returns color * intensity
        Color computeLight( const Geometry::HitRecord& hit, const std::vector<std::shared_ptr<APrimitive>>& primitives) const override;
    };
} // namespace RayTracer