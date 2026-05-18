#include "Livescene.hpp"
#include "../../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../../Utils/Color.hpp"
#include <algorithm>
#include <chrono>

namespace
{
    constexpr std::chrono::milliseconds kFramePollDelay{32};
}

namespace RayTracer
{
    void Livescene::renderSceneAsync(Scene& scene,
        std::shared_ptr<FrameBuffer> frameBuffer,
        std::shared_ptr<std::exception_ptr> error)
    {
        try {
            if (_renderer == nullptr)
                throw RayTracer::Exception("Livescene renderer is not attached.");
            _renderer->render(scene, *frameBuffer);
        } catch (...) {
            *error = std::current_exception();
        }
    }

    sf::Texture Livescene::framebufferToTexture(const FrameBuffer& frameBuffer)
    {
        sf::Texture texture;
        if (frameBuffer.empty() || frameBuffer[0].empty())
            return texture;

        const unsigned int width = static_cast<unsigned int>(frameBuffer[0].size());
        const unsigned int height = static_cast<unsigned int>(frameBuffer.size());

        sf::Image image;
        image.create(width, height, sf::Color::Black);

        for (unsigned int y = 0; y < height; ++y) {
            for (unsigned int x = 0; x < width; ++x) {
                const Color& color = frameBuffer[y][x];
                const sf::Uint8 red = static_cast<sf::Uint8>(std::clamp(color.r, 0.0, 1.0) * 255.0);
                const sf::Uint8 green = static_cast<sf::Uint8>(std::clamp(color.g, 0.0, 1.0) * 255.0);
                const sf::Uint8 blue = static_cast<sf::Uint8>(std::clamp(color.b, 0.0, 1.0) * 255.0);
                image.setPixel(x, y, sf::Color(red, green, blue));
            }
        }

        texture.loadFromImage(image);
        return texture;
    }

    void Livescene::handleWindowEvents(sf::RenderWindow& window)
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed &&
                (event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape))
                window.close();
        }
    }

    void Livescene::displayFrame(sf::RenderWindow& window, const sf::Texture& texture)
    {
        sf::Sprite sprite(texture);
        const sf::Vector2u size = window.getSize();
        const sf::Vector2u textureSize = texture.getSize();

        if (textureSize.x > 0 && textureSize.y > 0) {
            sprite.setScale(
                static_cast<float>(size.x) / static_cast<float>(textureSize.x),
                static_cast<float>(size.y) / static_cast<float>(textureSize.y));
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    void Livescene::execute(Scene& scene, std::map<std::string, std::string> &parameters)
    {
        if (_renderer == nullptr)
            throw RayTracer::Exception("Livescene renderer is not attached.");
            
        auto frameBuffer = std::make_shared<FrameBuffer>();
        auto renderError = std::make_shared<std::exception_ptr>();
        std::thread renderThread(&Livescene::renderSceneAsync, this,
            std::ref(scene), frameBuffer, renderError);
        pthread_t renderThreadHandle = renderThread.native_handle();

        sf::RenderWindow window(
            sf::VideoMode(_renderer->getWidth(), _renderer->getHeight()),
            "LiveScene",
            sf::Style::Titlebar | sf::Style::Close);
        window.setVerticalSyncEnabled(true);
        sf::Texture texture;
        while (window.isOpen()) {
            handleWindowEvents(window);
            texture = framebufferToTexture(*frameBuffer);
            displayFrame(window, texture);
            std::this_thread::sleep_for(kFramePollDelay);
        }
        std::this_thread::sleep_for(kFramePollDelay);
        if (renderThread.joinable())
            renderThread.join();
        if (*renderError)
            std::rethrow_exception(*renderError);
    }
} // namespace RayTracer
