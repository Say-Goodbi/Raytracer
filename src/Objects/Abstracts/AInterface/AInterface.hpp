#pragma once
#include "../ARenderer/ARenderer.hpp"

namespace RayTracer
{
    class Scene;

    class AInterface
    {
    protected:
        ARenderer* _renderer;

    public:
        AInterface(ARenderer* renderer) : _renderer(renderer) {}
        virtual ~AInterface() = default;

        /**
         * Execute the interface's behavior (e.g., render a scene and present it)
         */
        virtual void execute(Scene& scene) = 0;
    };
} // namespace RayTracer
