#include "../../Plugin.hpp"
#include "Interfaces/Livescene/Livescene.hpp"

extern "C"
{
    std::string getName() { return "LivescenePlugin"; }

    std::map<std::string, std::function<RayTracer::Component(RayTracer::NodePtr)>> getInitializers()
    {
        return {
            {"LiveScene", [](RayTracer::NodePtr) -> RayTracer::Component {
                return std::static_pointer_cast<RayTracer::AInterface>(std::make_shared<RayTracer::Livescene>());
            }}
        };
    }
}