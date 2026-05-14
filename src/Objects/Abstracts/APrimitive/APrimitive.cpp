#include "APrimitive.hpp"

namespace RayTracer
{
    IMaterial *APrimitive::getMaterial() const
    {
        return this->_material.get();
    }
} // namespace RayTracer
