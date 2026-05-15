/*******************************************************
 * @file IPlugin.hpp
 * @brief Interface for plugins in the ray tracer application
 * 
 * @details This file defines the IPlugin interface, which all plugins must implement to be compatible with the ray tracer application. 
 * The IPlugin interface requires plugins to provide a name and a set of initializers for registering new primitives, materials, lights, renderers, or interfaces.
 *******************************************************/

#pragma once

#include <functional>
#include <memory>
#include <variant>
#include <map>
#include "../Exceptions/ParsingException/ParsingException.hpp"
#include "Abstracts/AInterface/AInterface.hpp"
#include "Abstracts/APrimitive/APrimitive.hpp"
#include "Abstracts/ARenderer/ARenderer.hpp"
#include "Abstracts/ILight.hpp"
#include "Abstracts/IMaterial.hpp"

namespace RayTracer
{
    typedef struct Node Node;
    typedef std::variant<std::shared_ptr<AInterface>, std::shared_ptr<APrimitive>, std::shared_ptr<ARenderer>, std::shared_ptr<ILight>, std::shared_ptr<IMaterial>> Component; ///< Type alias for any pluggable class type (interface, primitive, renderer, light, material) that a plugin can register
    typedef std::variant<int, long long, float, double, std::string, bool, Component> ScalarValue;
    typedef std::unique_ptr<Node> NodePtr;
    typedef std::map<std::string, NodePtr> Object;
    typedef std::vector<Object> VectorObject;
    
    struct Node
    {
        std::variant<ScalarValue, Object, VectorObject> value;
    };
} // namespace RayTracer