#include "SceneWriter.hpp"
#include "../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../Utils/Color.hpp"
#include <fstream>

namespace RayTracer
{
    SceneWriter::SceneWriter(ARenderer* renderer, const std::string& outputFile)
    : AInterface(renderer), _outputFile(outputFile)
    {
    }

    void SceneWriter::execute(Scene& scene)
    {
        auto frameBuffer = _renderer->render(scene);
        if (frameBuffer.empty())
            return;

        int height = static_cast<int>(frameBuffer.size());
        int width = static_cast<int>(frameBuffer[0].size());

        std::ofstream ofs(_outputFile, std::ios::out);
        if (!ofs)
            return;

        ofs << "P3\n" << width << " " << height << "\n255\n";
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const Color &c = frameBuffer[y][x];
                ofs << static_cast<int>(c.r * 255) << " "
                    << static_cast<int>(c.g * 255) << " "
                    << static_cast<int>(c.b * 255) << "\n";
            }
        }

        ofs.close();
    }
} // namespace RayTracer
