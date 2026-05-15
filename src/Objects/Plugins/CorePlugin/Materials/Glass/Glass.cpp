#include "Glass.hpp"

namespace RayTracer
{
    Color Glass::evaluate(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const
    {
        return _albedo * (1.0 / M_PI);
    }
    
    float Glass::pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const
    {
        return 0.0f;
    }

    Color Glass::emission() const
    {
        return Color(0, 0, 0);
    }

    Geometry::Vector3D Glass::sample(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir) const
    {
        float cosi = std::clamp(static_cast<float>(viewDir.dot(normal)), -1.0f, 1.0f);
        float etai = 1.0f, etat = _refractiveIndex;
        Geometry::Vector3D n = normal;
        if (cosi < 0) {
            cosi = -cosi;
            std::swap(etai, etat);
            n = normal * -1.0f;
        }
        float eta = etai / etat;
        float k = 1.0f - eta * eta * (1.0f - cosi * cosi);
        if (k < 0)
            return n * (2.0f * cosi) - viewDir;
        else
            return viewDir * (-eta) + n * (eta * cosi - std::sqrt(k));
    }


} // namespace RayTracer
