#pragma once
#include "../../../Utils/Color.hpp"
#include <vector>

namespace RayTracer
{
    // Forward declarations to avoid circular dependencies
    class Scene;

    class ARenderer
    {
    protected:
        int _width;
        int _height;

    public:
        ARenderer(int width, int height) : _width(width), _height(height) {}
        
        virtual ~ARenderer() = default;

        /**
         * @brief Renders a scene and returns the produced frame buffer
         * @param scene The scene to render
         * @return 2D frame buffer (rows x columns)
         */
        virtual std::vector<std::vector<Color>> render(Scene& scene) = 0;

        int getWidth() const { return _width; }
        int getHeight() const { return _height; }
    };
} // namespace RayTracer
