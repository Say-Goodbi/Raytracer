#include "Plane.hpp"

namespace RayTracer
{
    /**
     * @brief Test ray–plane intersection using the algebraic plane equation.
     *
     * Algorithm: Solve for intersection parameter t along the ray.
     * Ray equation: P(t) = ray.origin + t * ray.direction
     * Plane equation: (P - _point) · _normal = 0
     *
     * Substituting the ray into the plane equation:
     *   t = ((_point - ray.origin) · _normal) / (ray.direction · _normal)
     *
     * Steps:
     * 1. Compute denominator = ray.direction · _normal
     *    - If |denominator| < 1e-6: ray is parallel to plane (no intersection)
     * 2. Compute numerator = (_point - ray.origin) · _normal
     * 3. Solve t = numerator / denominator
     * 4. If t > 1e-4: intersection is in front of ray origin (epsilon avoids self-intersection)
     * 5. Flip normal toward the incoming ray (for correct back-face shading)
     * 6. Return HitRecord with intersection point, normal, distance, and material
     *
     * @param ray The ray to test against the plane
     * @return Optional HitRecord if intersection occurs with t >= 0, empty otherwise
     */
    std::optional<Geometry::HitRecord> Plane::hit(const Geometry::Ray& ray) const
    {
        float denom = _normal.dot(ray.direction);
        if (std::abs(denom) > 1e-6) {
            float t = (_point - ray.origin).dot(_normal) / denom;
            if (t > 1e-4f) {
                Geometry::Vector3D outwardNormal = denom < 0 ? _normal : _normal * -1;
                Geometry::HitRecord hit(ray.at(t), outwardNormal, t, this->getMaterial());
                return hit;
            }
        }
        return std::nullopt;
    }
} // namespace RayTracer