#include "SceneWriter.hpp"
#include "../../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../../Utils/Color.hpp"
#include <fstream>

namespace RayTracer
{
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
    void SceneWriter::execute(Scene& scene, std::map<std::string, std::string> &parameters)
    {
        std::string outputFile = parameters.find("o") != parameters.end() ? parameters.find("o")->second : "output.ppm";
        int nb_threads = parameters.find("t") != parameters.end() ? atoi(parameters.find("t")->second.c_str()) : 1;

        if (nb_threads < 1)
            throw RayTracer::Exception("Invalid number of threads specified. Must be at least 1.");

        std::vector<std::vector<RayTracer::Color>> frameBuffer = _renderer->render(scene);
        if (frameBuffer.empty())
            return;
        int height = static_cast<int>(frameBuffer.size());
        int width = static_cast<int>(frameBuffer[0].size());

        std::ofstream ofs(outputFile, std::ios::out);
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
        std::cout << "Scene rendered and written to " << outputFile << std::endl;
    }
} // namespace RayTracer
