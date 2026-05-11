#pragma once
#include "../../Objects/Plugin.hpp"
#include "../PluginManager/PluginManager.hpp"
#include <vector>
#include <memory>

namespace RayTracer
{
    /// Represents a scene containing a camera, primitives, and lights.
    class Scene
    {
        private:
            std::shared_ptr<ARenderer> _renderer;                  ///< Active camera/renderer
            std::vector<std::shared_ptr<APrimitive>> _primitives;  ///< Renderable objects
            std::vector<std::shared_ptr<ILight>> _lights;          ///< Light sources
        protected:
        public:
            /// Constructor.
            /// @param camera The renderer for this scene
            Scene(ARenderer& camera);
            Scene(void);
            ~Scene() = default;

            /// Add a primitive (shape) to the scene.
            /// @param primitive Shared pointer to a primitive
            void addPrimitive(std::shared_ptr<APrimitive> primitive);

            /// Add primitives to the scene.
            /// @param primitiveSettings Configuration settings for the primitives to add
            /// @param scene Reference to the scene to add primitives to
            void addPrimitives(RayTracer::NodePtr &primitives, PluginManager &pluginManager);

            /// Add a light to the scene.
            /// @param light Shared pointer to a light
            void addLight(std::shared_ptr<ILight> light);

            /// Add lights to the scene.
            /// @param lightSettings Configuration settings for the lights to add
            /// @param scene Reference to the scene to add lights to
            void addLights(RayTracer::NodePtr &lights, PluginManager &pluginManager);

            /// Get the scene's camera.
            /// @return Reference to the camera
            const std::shared_ptr<ARenderer>& getRenderer() const;

            /// Get mutable reference to primitives.
            /// @return Reference to the primitives vector
            std::vector<std::shared_ptr<APrimitive>>& getPrimitives();

            /// Get mutable reference to lights.
            /// @return Reference to the lights vector
            std::vector<std::shared_ptr<ILight>>& getLights();

            /// Update the scene's camera.
            /// @param newCamera Reference to the new camera to set
            void updateCamera(RayTracer::NodePtr &camera, PluginManager &pluginManager);

    };
} // namespace RayTracer
