#include "Cylinder.hpp"
#include <cmath>

namespace RayTracer
{
    /**
     * @brief Test ray–cylinder intersection using the quadratic form of the cylinder equation.
     *
     * Algorithm: Solve for intersection parameter t along the ray.
     * Ray equation: P(t) = ray.origin + t * ray.direction
     * Infinite cylinder equation: |P - _origin - ((P - _origin) · axis) * axis|² = _radius²
     *
     * Decompose the ray and delta into components perpendicular to the cylinder axis:
     *   aVec = ray.direction - (ray.direction · axis) * axis  (perpendicular direction)
     *   bVec = delta         - (delta · axis) * axis           (perpendicular offset)
     *
     * This yields a quadratic equation in t:
     *   A*t² + B*t + C = 0
     * where:
     *   A = aVec · aVec
     *   B = 2 * (aVec · bVec)
     *   C = bVec · bVec - radius²
     *
     * Steps:
     * 1. Normalize the cylinder axis
     * 2. Compute delta = ray.origin - _origin
     * 3. Project ray direction and delta onto the axis to get perpendicular components
     * 4. If A < 1e-10: ray is parallel to the cylinder axis (no lateral intersection)
     * 5. Compute discriminant = B² - 4*A*C
     *    - If disc < 0: ray misses the cylinder entirely
     * 6. Try nearest root t = (-B - sqrt(disc)) / (2*A); fall back to far root if t < 1e-4
     * 7. Compute hit point and outward normal as the radial unit vector from the axis point
     * 8. Return HitRecord with intersection point, normal, distance, and material
     *
     * @param ray The ray to test against the cylinder
     * @return Optional HitRecord if intersection occurs with t >= 1e-4, empty otherwise
     */
    std::optional<Geometry::HitRecord> Cylinder::hit(const Geometry::Ray& ray) const
    {
        Geometry::Vector3D axis = _axis.normalize();
        Geometry::Vector3D delta = ray.origin - _origin;

        double dAxisDir = axis.dot(ray.direction);
        double dAxisDelta = axis.dot(delta);

        Geometry::Vector3D aVec = ray.direction - axis * dAxisDir;
        Geometry::Vector3D bVec = delta - axis * dAxisDelta;
        double A = aVec.dot(aVec);
        if (A < 1e-10)
            return std::nullopt;

        double B = 2.0 * aVec.dot(bVec);
        double C = bVec.dot(bVec) - static_cast<double>(_radius) * _radius;
        double disc = B * B - 4.0 * A * C;
        if (disc < 0.0)
            return std::nullopt;
        double sqrtDisc = std::sqrt(disc);
        double t = (-B - sqrtDisc) / (2.0 * A);
        if (t < 1e-4) {
            t = (-B + sqrtDisc) / (2.0 * A);
            if (t < 1e-4)
                return std::nullopt;
        }
        Geometry::Point3D hitPoint = ray.at(static_cast<float>(t));
        double proj = axis.dot(hitPoint - _origin);
        Geometry::Point3D axisPoint = _origin + axis * proj;
        Geometry::Vector3D outwardNormal = (hitPoint - axisPoint) / static_cast<double>(_radius);
        return Geometry::HitRecord(hitPoint, outwardNormal, t, this->getMaterial());
    }
} // namespace RayTracer
