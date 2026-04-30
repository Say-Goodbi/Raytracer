#pragma once
#include "../ARenderer/ARenderer.hpp"

namespace RayTracer
{
    class Scene;

    /// Abstract base class for application interfaces.
    /// An interface owns a renderer and orchestrates execution (e.g., rendering and output).
    class AInterface
    {
    protected:
        ARenderer* _renderer;  ///< Owned renderer instance

    public:
        /// Constructor.
        /// @param renderer The renderer to use for rendering scenes
        AInterface(ARenderer* renderer) : _renderer(renderer) {}
        virtual ~AInterface() = default;

        /// Execute the interface's main behavior (e.g., render and output a scene).
        /// @param scene The scene to process
        virtual void execute(Scene& scene) = 0;
    };
} // namespace RayTracer
