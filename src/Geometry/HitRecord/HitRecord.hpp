#pragma once

#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"
#include "../../Objects/Interface/IMaterial.hpp"

namespace Geometry
{
    struct HitRecord
    {
        Point3D point;
        Vector3D normal;
        float rayDistance;
        RayTracer::IMaterial *material;

        HitRecord() : point(), normal(), rayDistance(0), material(nullptr) {}
        HitRecord(const Point3D& point, const Vector3D& normal, float rayDistance, RayTracer::IMaterial *material)
            : point(point), normal(normal), rayDistance(rayDistance), material(material) {}
    };
} // namespace Geometry