#include "Color.hpp"


namespace RayTracer
{
    Color Color::clamp(void)
    {
        r = std::min(std::max(r, 0.0), 1.0);
        g = std::min(std::max(g, 0.0), 1.0);
        b = std::min(std::max(b, 0.0), 1.0);
        return *this;
    }
} // namespace RayTracer