#include "APrimitive.hpp"

namespace RayTracer
{
    APrimitive::APrimitive(IMaterial *material) : _material(material)
    {
    }

    IMaterial *APrimitive::getMaterial() const
    {
        return this->_material;
    }

} // namespace RayTracer
