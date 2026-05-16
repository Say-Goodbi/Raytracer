#pragma once
#include "../../../../Plugin.hpp"
#include <atomic>
#include <thread>
#include <SFML/Graphics.hpp>

namespace RayTracer
{
    /// Concrete interface implementation that renders a scene in an SFML window.
    class Livescene : public AInterface
    {
    private:
        using FrameBuffer = std::vector<std::vector<Color>>;

        void renderSceneAsync(Scene& scene,
            std::shared_ptr<FrameBuffer> frameBuffer,
            std::shared_ptr<std::exception_ptr> error);

        sf::Texture framebufferToTexture(const FrameBuffer& frameBuffer);
        void handleWindowEvents(sf::RenderWindow& window);
        void displayFrame(sf::RenderWindow& window, const sf::Texture& texture);

    public:
        /// Constructor.
        Livescene() = default;
        ~Livescene() override = default;

        /// Render the scene (and the image build) and in a SFML graphical window.
        /// @param scene The scene to render
        /// @param parameters Additional parameters for rendering
        void execute(Scene& scene, std::map<std::string, std::string> &parameters) override;
    };
} // namespace RayTracer
