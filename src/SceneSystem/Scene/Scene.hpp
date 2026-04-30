#pragma once
#include "../Camera/Camera.hpp"
#include "../../Objects/Interface/IPrimitive.hpp"
#include "../../Objects/Interface/ILight.hpp"
#include <vector>
#include <memory>
namespace RayTracer
{
    class Scene
    {
        private:
            Camera _camera;
            std::vector<std::unique_ptr<IPrimitive>> _primitives;
            std::vector<std::unique_ptr<ILight>> _lights;
        protected:
        public:
            Scene(Camera camera);
            ~Scene() = default;

            void addPrimitive(std::unique_ptr<IPrimitive> primitive);
            void addLight(std::unique_ptr<ILight> light);
            const Camera& getCamera() const;
            std::vector<std::unique_ptr<IPrimitive>>& getPrimitives();
            std::vector<std::unique_ptr<ILight>>& getLights();

    };
} // namespace RayTracer
