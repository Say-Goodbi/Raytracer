#pragma once

#include "../../../Geometry/HitRecord/HitRecord.hpp"
#include "../../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../../Utils/Color.hpp"

namespace RayTracer
{
    class FlatColor
    {
        protected:
            Color _color;
        public: 
            FlatColor(const Color& color) : _color(color) {}
            virtual ~FlatColor() = default;
            virtual Color computeColor(Geometry::HitRecord& hit, std::vector<ILight*>& lights) = 0;
    };
} // namespace RayTracer
