#include "Phong.hpp"

namespace RayTracer
{

    Color Phong::evaluate(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const
    {
        Color diffuse = _albedo * (1.0f / M_PI);

        Geometry::Vector3D reflectDir = (normal * 2.0f * normal.dot(lightDir) - lightDir).normalize();
        float RdotV = std::max(0.0f, static_cast<float>(reflectDir.dot(viewDir)));
        float normalization = (_shininess + 2.0f) / (2.0f * M_PI);
        Color specular = _albedo * (normalization * std::pow(RdotV, _shininess));

        return diffuse + specular;
    }

    Geometry::Vector3D Phong::sample(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir) const
    {
        Geometry::Vector3D reflectDir = (normal * 2.0f * normal.dot(viewDir) - viewDir).normalize();

        constexpr float u1 = 0.5f;
        constexpr float u2 = 0.5f;
        float cosTheta = std::pow(u1, 1.0f / (_shininess + 1.0f));
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
        float phi = 2.0f * M_PI * u2;
        Geometry::Vector3D local(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);

        Geometry::Vector3D up = std::abs(reflectDir.x) > 0.9f ? Geometry::Vector3D(0, 1, 0) : Geometry::Vector3D(1, 0, 0);
        Geometry::Vector3D tangent = Geometry::Vector3D::cross(up, reflectDir).normalize();
        Geometry::Vector3D binormal = Geometry::Vector3D::cross(reflectDir, tangent);

        return Geometry::Vector3D(
            local.x * tangent.x + local.y * binormal.x + local.z * reflectDir.x,
            local.x * tangent.y + local.y * binormal.y + local.z * reflectDir.y,
            local.x * tangent.z + local.y * binormal.z + local.z * reflectDir.z
        ).normalize();
    }

    float Phong::pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D& viewDir, const Geometry::Vector3D& lightDir) const
    {
        Geometry::Vector3D reflectDir = (normal * 2.0f * normal.dot(viewDir) - viewDir).normalize();
        float RdotL = std::max(0.0f, static_cast<float>(reflectDir.dot(lightDir)));
        return (_shininess + 1.0f) / (2.0f * M_PI) * std::pow(RdotL, _shininess);
    }

    Color Phong::emission() const
    {
        return Color(0, 0, 0);
    }
} // namespace RayTracer

