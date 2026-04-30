#pragma once
#include "../Geometry/Ray/Ray.hpp"
#include "../Objects/Interface/IMaterial.hpp"
#include "../Objects/Interface/ILight.hpp"
#include "../SceneSystem/Scene/Scene.hpp"
#include <iostream>

#define OUTPUT_FILE "output.ppm"

namespace RayTracer
{
    class Renderer
    {
        private:
            int _width;
            int _height;
            std::vector<std::vector<Color>> _frameBuffer;

            Color castRay(Geometry::Ray& ray, Scene& scene);
            bool computeShadow(Geometry::HitRecord& hit, Scene& scene);
            void writeOutput();
        protected:
        public:
            Renderer(int width, int height) : _width(width), _height(height) {}
            ~Renderer() = default;
            void render(Scene& scene);
    };
} // namespace RayTracer
