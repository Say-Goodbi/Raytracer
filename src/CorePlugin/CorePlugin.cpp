#include "CorePlugin.hpp"
#include "Interfaces/SceneWriter/SceneWriter.hpp"
#include "Materials/FlatColor.hpp"

std::string CorePlugin::getName() const
{
    return "CorePlugin";
}

std::map<std::string, std::function<Component(Setting)>> CorePlugin::getInitializers() const
{
    return {
        {"SceneWriter", [](Setting setting) -> Component {
            auto settingsMap = std::get<std::map<std::string, Setting>>(setting);
            auto renderer = std::get<ARenderer*>(settingsMap["renderer"]);
            auto outputFile = std::get<std::string>(settingsMap["outputFile"]);
            return SceneWriter(renderer, outputFile);
        }},
        {"FlatColor", [](Setting setting) -> Component {
            auto settingsMap = std::get<std::map<std::string, Setting>>(setting);
            auto color = std::get<Color>(settingsMap["color"]);
            return FlatColor(color);
        }}
    };
}