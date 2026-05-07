#pragma once
#include "../../../Objects/IPlugin.hpp"

/**
 * @class CorePlugin
 * @brief Built-in plugin providing core raytracer functionality.
 *
 * The CorePlugin is the primary plugin for the raytracer system.
 *
 * This plugin is always loaded and provides the essential components needed
 * for basic raytracing output and shading.
 */
class CorePlugin : public IPlugin
{
    public:
        ~CorePlugin() override = default;

        /**
         * @brief Get the plugin name.
         *
         * @return "CorePlugin"
         */
        std::string getName() const override;

        /**
         * @brief Get the component initializers provided by this plugin.
         *
         * Returns a map of component names to factory functions. Each factory
         * function takes a Setting (parameter dictionary) and returns a Component.
         *
         * Provided initializers:
         * - **"SceneWriter"** — Creates a SceneWriter interface (renderer → PPM file)
         * - **"FlatColor"** — Creates a FlatColor material (constant RGB shading)
         *
         * @return Map of component name → initializer function
         */
        std::map<std::string, std::function<Component(Setting)>> getInitializers() const override;
};