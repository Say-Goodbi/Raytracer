#pragma once
#include "../../../Geometry/HitRecord/HitRecord.hpp"
#include "../../../Geometry/Rectangle3D/Rectangle3D.hpp"
#include "../../../Utils/Color.hpp"

namespace RayTracer
{
    class FlatColor : public IMaterial
    {
        protected:
            Color _color;
        public: 
            FlatColor(const Color& color) : _color(color) {};
            ~FlatColor() = default;
            Color computeColor(Geometry::HitRecord& hit, std::vector<ILight*>& lights) override
            {
                return this->_color;
            }
    };
} // namespace RayTracer
