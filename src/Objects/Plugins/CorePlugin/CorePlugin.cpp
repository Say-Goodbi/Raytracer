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
                 RayTracer::Object settingsMap = std::get<RayTracer::Object>(node->value);
                 auto rendererPtr = std::get<std::shared_ptr<ARenderer>>(std::get<RayTracer::ScalarValue>(settingsMap.at("renderer")));
                 auto outputFile = std::get<std::string>(std::get<RayTracer::ScalarValue>(settingsMap.at("outputFile")));
                 auto sw = std::make_shared<SceneWriter>(rendererPtr.get(), outputFile);
                 return std::static_pointer_cast<RayTracer::AInterface>(sw);
             }},
            {"FlatColor", [](NodePtr node) -> Component
             {
                 RayTracer::Object settingsMap = std::get<RayTracer::Object>(node->value);
                 auto color = std::get<Color>(std::get<RayTracer::ScalarValue>(settingsMap.at("color")));
                 auto mat = std::make_shared<FlatColor>(color);
                 return std::static_pointer_cast<RayTracer::IMaterial>(mat);
             }},
             {"planes", [](NodePtr node) -> Component
              {

              }}
            };
    }
}
