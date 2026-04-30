#pragma once
#include "../../Geometry/HitRecord/HitRecord.hpp"
#include "../../Utils/Color.hpp"
#include "ILight.hpp"
#include <vector>

namespace RayTracer
{
    class IMaterial
    {
    public:
        virtual Color computeColor(Geometry::HitRecord& hit, std::vector<ILight*>& lights) = 0;
        virtual ~IMaterial() = default;
    };
} // namespace RayTracer
