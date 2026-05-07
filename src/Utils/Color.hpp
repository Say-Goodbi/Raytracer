#pragma once
#include <algorithm>
namespace RayTracer
{
    struct Color
    {
        double r;
        double g;
        double b;
        Color(const Color& color) = default;
        Color() : r(0), g(0), b(0) {}
        Color(double r, double g, double b) : r(r), g(g), b(b) {}

        Color clamp(void);

        Color operator*(double scalar) const
        {
            return Color(r * scalar, g * scalar, b * scalar);
        }

        Color operator*(const Color &other) const
        {
            return Color(r * other.r, g * other.g, b * other.b);
        }

        Color operator+(const Color &other) const
        {
            return Color(r + other.r, g + other.g, b + other.b);
        }

        Color operator-(const Color &other) const
        {
            return Color(r - other.r, g - other.g, b - other.b);
        }
    };
} // namespace RayTracer
