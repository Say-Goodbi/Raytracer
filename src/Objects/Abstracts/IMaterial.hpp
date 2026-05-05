#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "ILight.hpp"
#include <vector>

namespace RayTracer
{
    /// Abstract base for materials defining how surfaces shade.
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;
        // 1. Evaluate BRDF (local light response)
        virtual Color evaluate(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir, const Geometry::Vector3D &lightDir) const = 0;
        // 2. Sample a bounce direction
        virtual Geometry::Vector3D sample(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir, float u1, float u2) const = 0;
        // 3. Probability of the sampled direction
        virtual float pdf(const Geometry::Vector3D &normal, const Geometry::Vector3D &viewDir, const Geometry::Vector3D &sampleDir) const = 0;
        // 4. Emission (for light materials)
        virtual Color emission() const = 0;
    };
} // namespace RayTracer
