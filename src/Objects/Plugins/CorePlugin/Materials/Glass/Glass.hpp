#pragma once
#include "../../../../../Objects/Abstracts/AMaterial/AMaterial.hpp"

namespace RayTracer
{
    class Glass : public AMaterial
    {
        private:
            float _refractiveIndex;
        protected:
        public:
            Glass(const Color& albedo, float refractiveIndex = 1.5f) : AMaterial(albedo), _refractiveIndex(refractiveIndex) {}
            ~Glass() = default;

            Color evaluate(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const override;
            Geometry::Vector3D sample(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir) const override;
            float pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const override;
            Color emission() const override;
    };
} // namespace RayTracer

