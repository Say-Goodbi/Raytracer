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
        AInterface() = default;
        virtual ~AInterface() = default;

        /// Attach the renderer used by the interface.
        void setRenderer(ARenderer* renderer) { _renderer = renderer; }

        /// Execute the interface's main behavior (e.g., render and output a scene).
        /// @param scene The scene to process
        virtual void execute(Scene& scene, std::map<std::string, std::string> &parameters) = 0;
    };
} // namespace RayTracer
