#include "Lambertian.hpp"

namespace RayTracer
{
    Lambertian::Lambertian(const Color& albedo) : _albedo(albedo) {}
    Lambertian::~Lambertian() = default;

    Color Lambertian::evaluate(const Geometry::Vector3D&, const Geometry::Vector3D&, const Geometry::Vector3D&) const
    {
        return _albedo * (1.0 / M_PI);
    }

    Geometry::Vector3D Lambertian::sample(const Geometry::Vector3D& normal, const Geometry::Vector3D&) const
    {
        float u1 = 0.5f; // ADDED THESES FLOATS SINCE THE SAMPLE FUNCTION IS NOW DETERMINISTIC, WE CAN USE FIXED VALUES FOR u1 AND u2
        float u2 = 0.5f;
        float phi = 2.0f * M_PI * u1;
        float sinTheta = std::sqrt(1.0f - u2);
        Geometry::Vector3D local(sinTheta * std::cos(phi), sinTheta * std::sin(phi), std::sqrt(u2));

        // Build coordinate system around normal
        Geometry::Vector3D up = std::abs(normal.x) > 0.9 ? Geometry::Vector3D(0, 1, 0) : Geometry::Vector3D(1, 0, 0);
        Geometry::Vector3D tangent = Geometry::Vector3D::cross(up, normal).normalize();
        Geometry::Vector3D binormal = Geometry::Vector3D::cross(normal, tangent);

        return Geometry::Vector3D(
            local.x * tangent.x  + local.y * binormal.x + local.z * normal.x,
            local.x * tangent.y  + local.y * binormal.y + local.z * normal.y,
            local.x * tangent.z  + local.y * binormal.z + local.z * normal.z
        ).normalize();
    }

    float Lambertian::pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D&, const Geometry::Vector3D& sampleDir) const
    {
        return std::max(0.0f, (float)normal.dot(sampleDir)) / M_PI;
    }
} // namespace RayTracer
