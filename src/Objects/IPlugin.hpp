/*******************************************************
 * @file IPlugin.hpp
 * @brief Interface for plugins in the ray tracer application
 * 
 * @details This file defines the IPlugin interface, which all plugins must implement to be compatible with the ray tracer application. 
 * The IPlugin interface requires plugins to provide a name and a set of initializers for registering new primitives, materials, lights, renderers, or interfaces.
 *******************************************************/

#pragma once

#include <functional>
#include <variant>
#include "../Exceptions/RaytracerException/RaytracerException.hpp"
#include "Abstracts/AInterface.hpp"
#include "Abstracts/APrimitive/APrimitive.hpp"
#include "Abstracts/ARenderer/ARenderer.hpp"
#include "Abstracts/ILight.hpp"
#include "Abstracts/IMaterial.hpp"

typedef std::variant<AInterface, APrimitive, ARenderer, ILight, IMaterial> Component; ///< Type alias for any pluggable class type (interface, primitive, renderer, light, material) that a plugin can register
typedef std::variant<int, float, std::string, bool, std::vector<Setting>, std::map<std::string, Setting>> Setting; ///< Type alias for a setting value, which can be an int, float, string, bool, or a vector of settings (for nested configurations)

class IPlugin
{
    public:
        virtual ~IPlugin() = default; ///< Virtual destructor for proper cleanup of derived classes

        /// Get the name of the plugin.
        /// @return The name of the plugin as a string
        virtual std::string getName() const = 0;

        /// Get the initializers for the plugin, which are used to register new components (interfaces, primitives, renderers, lights, materials) with the application.
        /// @return A map where the key is the component name and the value is a function that takes a Setting and returns a Component instance
        virtual std::map<std::string, std::function<Component(Setting)>> getInitializers() const = 0; ///< Pure virtual function to retrieve plugin initializers (e.g., for registering new primitives, materials, etc.)
};