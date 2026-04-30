#pragma once

#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"
namespace Geometry
{
    class Rectangle3D
    {
    private:
    protected:
    public:
        Rectangle3D();
        Point3D origin;
        Vector3D width;
        Vector3D height;

        Point3D pointAt(double u, double v) const;
    };
} // namespace Geometry
