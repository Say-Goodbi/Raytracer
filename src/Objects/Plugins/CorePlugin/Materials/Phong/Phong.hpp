#pragma once

#include "../../../../../Objects/Abstracts/AMaterial/AMaterial.hpp"

namespace RayTracer
{
    class Phong : public AMaterial
    {
        private:
            float _shininess;
        protected:
        public:
            Phong(const Color& albedo, float shininess = 32.0f) : AMaterial(albedo), _shininess(shininess) {}
            ~Phong() = default;

            Color evaluate(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const override;
            Geometry::Vector3D sample(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir) const override;
            float pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const override;
            Color emission() const override;
    };
} // namespace RayTracer
