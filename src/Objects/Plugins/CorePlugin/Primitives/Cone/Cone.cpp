#include "Cone.hpp"
#include <cmath>

namespace RayTracer
{
    /**
     * @brief Test ray–cone intersection using the quadratic form of the cone equation.
     *
     * Algorithm: Solve for intersection parameter t along the ray.
     * Ray equation: P(t) = ray.origin + t * ray.direction
     * Cone surface equation: r_perp = k * h
     *   where h   = (P - _apex) · axis_hat  (signed distance along axis from apex)
     *         r_perp = |(P - _apex) - h * axis_hat|  (distance from axis)
     *         k   = tan(half_angle) = _radius / _height  (finite)
     *                               = _radius             (infinite, user-supplied slope)
     *
     * Decompose the ray and delta into components parallel/perpendicular to the axis:
     *   D_par  = ray.direction · axis_hat
     *   d_par  = delta · axis_hat          (delta = ray.origin - _apex)
     *   aVec   = ray.direction - axis_hat * D_par   (perpendicular component of direction)
     *   bVec   = delta         - axis_hat * d_par   (perpendicular component of origin offset)
     *
     * Substituting into r_perp² = k² * h² yields a quadratic in t:
     *   A * t² + B * t + C = 0
     * where:
     *   A = aVec · aVec  -  k² * D_par²
     *   B = 2 * (aVec · bVec  -  k² * D_par * d_par)
     *   C = bVec · bVec  -  k² * d_par²
     *
     * Height constraint at candidate t:
     *   h(t) = d_par + t * D_par
     *   - Infinite cone (_height == 0): accept if h > 1e-6  (forward lobe only)
     *   - Finite   cone (_height >  0): accept if 0 < h <= _height
     *
     * Outward surface normal:
     *   P_perp = hitPoint - _apex - h * axis_hat   (vector from axis to hit point)
     *   r_hat  = P_perp / |P_perp|                 (unit radial direction)
     *   normal = normalize(r_hat - k * axis_hat)
     *
     * Steps:
     * 1. Normalize the cone axis
     * 2. Compute k (slope) from geometry
     * 3. Compute delta, project onto axis for D_par and d_par
     * 4. Extract perpendicular components aVec, bVec
     * 5. Build quadratic coefficients A, B, C
     * 6. If |A| < 1e-10: ray is parallel to cone surface (degenerate), return no hit
     * 7. Compute discriminant = B² - 4*A*C; if disc < 0, ray misses
     * 8. Try nearest root t1 = (-B - sqrt(disc)) / (2*A); fall back to far root t2 if needed
     * 9. In tryHit: verify t >= 1e-4, compute h, apply height constraint, compute normal, return HitRecord
     *
     * @param ray The ray to test against the cone
     * @return Optional HitRecord if intersection occurs with t >= 1e-4, empty otherwise
     */
    std::optional<Geometry::HitRecord> Cone::hit(const Geometry::Ray& ray) const
    {
        Geometry::Vector3D axis = _axis.normalize();
        Geometry::Vector3D delta = ray.origin - _apex;

        double k = (_height > 0.0f)
            ? static_cast<double>(_radius) / static_cast<double>(_height)
            : static_cast<double>(_radius);
        double k2 = k * k;

        double D_par  = axis.dot(ray.direction);
        double d_par  = axis.dot(delta);

        Geometry::Vector3D aVec = ray.direction - axis * D_par;
        Geometry::Vector3D bVec = delta - axis * d_par;

        double A = aVec.dot(aVec) - k2 * D_par * D_par;
        double B = 2.0 * (aVec.dot(bVec) - k2 * D_par * d_par);
        double C = bVec.dot(bVec) - k2 * d_par * d_par;

        if (std::abs(A) < 1e-10)
            return std::nullopt;

        double disc = B * B - 4.0 * A * C;
        if (disc < 0.0)
            return std::nullopt;

        double sqrtDisc = std::sqrt(disc);
        double t1 = (-B - sqrtDisc) / (2.0 * A);
        double t2 = (-B + sqrtDisc) / (2.0 * A);

        auto tryHit = [&](double t) -> std::optional<Geometry::HitRecord> {
            if (t < 1e-4)
                return std::nullopt;
            double h = d_par + t * D_par;
            if (_height > 0.0f) {
                if (h <= 0.0 || h > static_cast<double>(_height))
                    return std::nullopt;
            } else {
                if (h <= 1e-6)
                    return std::nullopt;
            }
            Geometry::Point3D hitPoint = ray.at(static_cast<float>(t));
            Geometry::Vector3D P_perp = (hitPoint - _apex) - axis * h;
            double r_perp = P_perp.length();
            if (r_perp < 1e-10)
                return std::nullopt;
            Geometry::Vector3D r_hat = P_perp / r_perp;
            Geometry::Vector3D outwardNormal = (r_hat - axis * k).normalize();
            return Geometry::HitRecord(hitPoint, outwardNormal, t, this->getMaterial());
        };

        if (auto rec = tryHit(t1))
            return rec;
        return tryHit(t2);
    }

    std::optional<Geometry::AABB> Cone::getBounds() const
    {
        return std::nullopt;
    }
} // namespace RayTracer
