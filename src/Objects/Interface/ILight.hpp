#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "../../SceneSystem/Scene/Scene.hpp"

namespace RayTracer
{
    class ILight
    {
    public:
        virtual ~ILight() = default;
        virtual Color computeContribution(Geometry::HitRecord& hit, Scene& scene) = 0;
        virtual Geometry::Point3D getPosition() const = 0;
    };
} // namespace RayTracer
