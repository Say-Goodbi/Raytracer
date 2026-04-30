#pragma once
#include "../../../Abstracts/AInterface/AInterface.hpp"
#include <string>

namespace RayTracer
{
    class SceneWriter : public AInterface
    {
    private:
        std::string _outputFile;
    public:
        SceneWriter(ARenderer* renderer, const std::string& outputFile = "output.ppm");
        ~SceneWriter() override = default;

        void execute(Scene& scene) override;
    };
} // namespace RayTracer
