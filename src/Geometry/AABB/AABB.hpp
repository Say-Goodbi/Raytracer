#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

#include "../Point3D/Point3D.hpp"
#include "../Ray/Ray.hpp"

namespace Geometry
{
    class AABB
    {
    public:
        Point3D minimum;
        Point3D maximum;

        AABB() : minimum(), maximum() {}
        AABB(const Point3D &minimum, const Point3D &maximum) : minimum(minimum), maximum(maximum) {}

        static AABB unite(const AABB &lhs, const AABB &rhs)
        {
            return AABB(
                Point3D(
                    std::min(lhs.minimum.x, rhs.minimum.x),
                    std::min(lhs.minimum.y, rhs.minimum.y),
                    std::min(lhs.minimum.z, rhs.minimum.z)
                ),
                Point3D(
                    std::max(lhs.maximum.x, rhs.maximum.x),
                    std::max(lhs.maximum.y, rhs.maximum.y),
                    std::max(lhs.maximum.z, rhs.maximum.z)
                )
            );
        }

        Point3D centroid() const
        {
            return Point3D(
                (minimum.x + maximum.x) * 0.5,
                (minimum.y + maximum.y) * 0.5,
                (minimum.z + maximum.z) * 0.5
            );
        }

        double surfaceArea() const
        {
            const double dx = std::max(0.0, maximum.x - minimum.x);
            const double dy = std::max(0.0, maximum.y - minimum.y);
            const double dz = std::max(0.0, maximum.z - minimum.z);

            return 2.0 * (dx * dy + dx * dz + dy * dz);
        }

        bool hit(const Ray &ray, double tMin = 1e-4, double tMax = std::numeric_limits<double>::infinity()) const
        {
            for (int axis = 0; axis < 3; ++axis) {
                const double origin = axis == 0 ? ray.origin.x : axis == 1 ? ray.origin.y : ray.origin.z;
                const double direction = axis == 0 ? ray.direction.x : axis == 1 ? ray.direction.y : ray.direction.z;
                const double minBound = axis == 0 ? minimum.x : axis == 1 ? minimum.y : minimum.z;
                const double maxBound = axis == 0 ? maximum.x : axis == 1 ? maximum.y : maximum.z;

                if (std::abs(direction) <= 1e-12) {
                    if (origin < minBound || origin > maxBound)
                        return false;
                    continue;
                }

                const double invDirection = 1.0 / direction;

                double t0 = (minBound - origin) * invDirection;
                double t1 = (maxBound - origin) * invDirection;
                if (invDirection < 0.0)
                    std::swap(t0, t1);
                tMin = std::max(tMin, t0);
                tMax = std::min(tMax, t1);
                if (tMax <= tMin)
                    return false;
            }
            return true;
        }
    };
}