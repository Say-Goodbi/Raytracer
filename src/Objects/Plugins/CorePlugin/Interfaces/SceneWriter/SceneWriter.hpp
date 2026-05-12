#pragma once
#include "../../../../../Objects/Plugin.hpp"

namespace RayTracer
{
    /// Concrete interface implementation that renders a scene and writes output to a PPM file.
    class SceneWriter : public AInterface
    {
    private:
        std::string _outputFile;  ///< Output file path
    public:
        /// Constructor.
        /// @param renderer The renderer to use for rendering
        /// @param outputFile Output PPM file path (default "output.ppm")
        SceneWriter(ARenderer* renderer, const std::string& outputFile = "output.ppm");
        ~SceneWriter() override = default;

        /// Render the scene and write the framebuffer to a PPM file.
        /// @param scene The scene to render and output
        void execute(Scene& scene) override;
    };
} // namespace RayTracer
