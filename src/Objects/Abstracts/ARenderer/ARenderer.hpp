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

        /// Render a scene into the provided framebuffer and return it.
        /// Implementations should fill/resize `framebuffer` to (_height x _width)
        /// and return it.
        virtual std::vector<std::vector<Color>> render(Scene& scene, std::vector<std::vector<Color>>& framebuffer) = 0;

        /// Convenience: allocate a framebuffer of the renderer's size and call
        /// the two-argument render overload.
        virtual std::vector<std::vector<Color>> render(Scene& scene)
        {
            std::vector<std::vector<Color>> framebuffer(_height, std::vector<Color>(_width));
            return render(scene, framebuffer);
        }

        /// Get output width in pixels.
        int getWidth() const { return _width; }

        /// Get output height in pixels.
        int getHeight() const { return _height; }
    };
} // namespace RayTracer
