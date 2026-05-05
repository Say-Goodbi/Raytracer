#pragma once

#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"
#include "../../Objects/Abstracts/IMaterial.hpp"

namespace Geometry
{
    /// Record of a ray–primitive intersection.
    struct HitRecord
    {
        Point3D point;                      ///< Intersection point in world space
        Vector3D normal;                    ///< Surface normal at intersection
        float rayDistance;                  ///< Distance along ray to intersection
        RayTracer::IMaterial *material;     ///< Material at intersection

        HitRecord() : point(), normal(), rayDistance(0), material(nullptr) {}
        
        /// Constructor.
        /// @param point Intersection point
        /// @param normal Surface normal
        /// @param rayDistance Distance from ray origin
        /// @param material Material at hit location
        HitRecord(const Point3D& point, const Vector3D& normal, float rayDistance, RayTracer::IMaterial *material)
            : point(point), normal(normal), rayDistance(rayDistance), material(material) {}
    };
} // namespace Geometry
