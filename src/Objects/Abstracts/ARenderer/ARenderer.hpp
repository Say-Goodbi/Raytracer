#pragma once
#include "../../../Utils/Color.hpp"
#include <vector>

namespace RayTracer
{
    // Forward declarations to avoid circular dependencies
    class Scene;

    /// Abstract base class for all renderers.
    /// Defines the interface for rendering a scene and producing a framebuffer.
    class ARenderer
    {
    protected:
        int _width;   ///< Output width in pixels
        int _height;  ///< Output height in pixels

    public:
        /// Constructor.
        /// @param width Output width in pixels
        /// @param height Output height in pixels
        ARenderer(int width, int height) : _width(width), _height(height) {}
        
        virtual ~ARenderer() = default;

        /// Render a scene and produce a framebuffer.
        /// @param scene The scene to render
        /// @return 2D framebuffer with dimensions (height x width)
        virtual std::vector<std::vector<Color>> render(Scene& scene) = 0;

        /// Get output width in pixels.
        int getWidth() const { return _width; }

        /// Get output height in pixels.
        int getHeight() const { return _height; }
    };
} // namespace RayTracer
