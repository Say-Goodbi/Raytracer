#pragma once
#include "../Camera/Camera.hpp"
#include "../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include "../../Objects/Abstracts/ILight.hpp"
#include <vector>
#include <memory>

namespace RayTracer
{
    /// Represents a scene containing a camera, primitives, and lights.
    class Scene
    {
        private:
            Camera _camera;                                        ///< Active camera/renderer
            std::vector<std::unique_ptr<IPrimitive>> _primitives;  ///< Renderable objects
            std::vector<std::unique_ptr<ILight>> _lights;          ///< Light sources
        protected:
        public:
            /// Constructor.
            /// @param camera The camera for this scene
            Scene(Camera camera);
            ~Scene() = default;

            /// Add a primitive (shape) to the scene.
            /// @param primitive Unique pointer to a primitive
            void addPrimitive(std::unique_ptr<IPrimitive> primitive);

            /// Add a light to the scene.
            /// @param light Unique pointer to a light
            void addLight(std::unique_ptr<ILight> light);

            /// Get the scene's camera.
            /// @return Reference to the camera
            const Camera& getCamera() const;

            /// Get mutable reference to primitives.
            /// @return Reference to the primitives vector
            std::vector<std::unique_ptr<IPrimitive>>& getPrimitives();

            /// Get mutable reference to lights.
            /// @return Reference to the lights vector
            std::vector<std::unique_ptr<ILight>>& getLights();

    };
} // namespace RayTracer
