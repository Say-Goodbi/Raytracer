#include "SceneWriter.hpp"
#include "../../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../../Utils/Color.hpp"
#include <fstream>

namespace RayTracer
{
    /**
     * @brief Constructor.
     *
     * @param renderer Pointer to the renderer (ARenderer) that will generate the framebuffer
     * @param outputFile Path to the output PPM file to be written
     */
    SceneWriter::SceneWriter(ARenderer* renderer, const std::string& outputFile)
    : AInterface(renderer), _outputFile(outputFile)
    {
    }

    /**
     * @brief Render the scene and write the result to a PPM file.
     *
     * Output format: PPM (Portable Pixmap) P3 (ASCII) format
     * PPM structure:
     *   - Header: "P3\n"
     *   - Dimensions: "width height\n"
     *   - Max color value: "255\n"
     *   - Pixel data: R G B values (0-255) per line, one pixel per line
     *
     * Algorithm:
     * 1. Call renderer->render(scene) to get the framebuffer
     * 2. Extract framebuffer dimensions (height × width)
     * 3. Open output file for writing
     * 4. Write PPM header with dimensions
     * 5. Iterate through each pixel [y][x] and write RGB values (converted to 0-255 range)
     * 6. Close the file
     *
     * @param scene The scene to render and write
     */
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

        // Write PPM header
        ofs << "P3\n" << width << " " << height << "\n255\n";
        
        // Write pixel data: iterate y (row) then x (column)
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const Color &c = frameBuffer[y][x];
                // Convert color from [0,1] to [0,255] range
                ofs << static_cast<int>(c.r * 255) << " "
                    << static_cast<int>(c.g * 255) << " "
                    << static_cast<int>(c.b * 255) << "\n";
            }
        }

        ofs.close();
    }
} // namespace RayTracer
