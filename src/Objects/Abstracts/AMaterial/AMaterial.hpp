#pragma once

#include "../IMaterial.hpp"

namespace RayTracer
{
    /// Abstract base class for all materials.
    /// Defines the interface for evaluating the BRDF, sampling bounce directions, and emission.
    class AMaterial : public IMaterial
    {
        protected:
            Color _albedo;
        public:
            AMaterial() = default;
            AMaterial(const Color& albedo) : _albedo(albedo) {}
            ~AMaterial() override = default;
    };
} // namespace RayTracer
