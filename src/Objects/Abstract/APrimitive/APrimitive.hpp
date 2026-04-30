#pragma once
#include "../../Interface/IPrimitive.hpp"
#include "../../Interface/IMaterial.hpp"

namespace RayTracer
{
    class APrimitive : public IPrimitive
    {
    private:
        IMaterial *_material;

    public:
        APrimitive(IMaterial *material);
        virtual ~APrimitive() = default;    
        IMaterial *getMaterial() const override;
    };
} // namespace RayTracer
