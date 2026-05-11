#include "Interfaces/SceneWriter/SceneWriter.hpp"
#include "../../../../Materials/FlatColor.hpp"

extern "C"
{
    std::string getName() const { return "CorePlugin"; };

    std::map<std::string, std::function<Component(NodePtr)>> getInitializers()
    {
        return {
            {"SceneWriter", [](NodePtr node) -> Component
             {
                 auto settingsMap = std::get<RayTracer::Object>(node->value);
                 auto renderer = std::get<ARenderer>(std::get<RayTracer::ScalarValue>(settingsMap.at("renderer")));
                 auto outputFile = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("outputFile")));
                 return SceneWriter(renderer, outputFile);
             }},
            {"FlatColor", [](NodePtr node) -> Component
             {
                 auto settingsMap = std::get<RayTracer::Object>(node->value);
                 auto color = std::get<Color>(std::get<RayTracer::ScalarValue>(settingsMap.at("color")));
                 return FlatColor(color);
             }}};
    }
}
