#pragma once
#include "../../Geometry/Point3D/Point3D.hpp"
#include "../../Geometry/Vector3D/Vector3D.hpp"

namespace Geometry
{
    class Ray
    {
    private:
    protected:
    public:
        Ray() = default;
        ~Ray() = default;

        Ray(const Geometry::Point3D& origin, const Geometry::Vector3D& direction) : origin(origin), direction(direction) {}
        Point3D at(float t) const;
        Geometry::Point3D origin;
        Geometry::Vector3D direction;

    };
} // namespace Geometry
