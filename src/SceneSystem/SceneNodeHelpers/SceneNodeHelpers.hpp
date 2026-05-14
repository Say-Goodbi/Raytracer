#pragma once

#include <type_traits>
#include <utility>

#include "../../Objects/Plugin.hpp"
#include "../../Geometry/Point3D/Point3D.hpp"
#include "../../Geometry/TransformMatrix/TransformMatrix.hpp"
#include "../../Geometry/Vector3D/Vector3D.hpp"
#include "../../Utils/Color.hpp"

namespace Raytracer
{
    namespace Detail
    {
        inline const RayTracer::Object &asObject(const RayTracer::NodePtr &node)
        {
            if (!node || !std::holds_alternative<RayTracer::Object>(node->value))
                throw RayTracer::Exception("Expected an object node");
            return std::get<RayTracer::Object>(node->value);
        }

        inline const RayTracer::NodePtr &field(const RayTracer::Object &object, const std::string &name)
        {
            RayTracer::Object::const_iterator it = object.find(name);

            if (it == object.end() || !it->second)
                throw RayTracer::Exception("Missing required parameter: " + name);
            return it->second;
        }

        inline double numericFromScalar(const RayTracer::ScalarValue &value)
        {
            return std::visit([](const auto &scalar) -> double {
                using ValueType = std::decay_t<decltype(scalar)>;

                if constexpr (std::is_same_v<ValueType, int> || std::is_same_v<ValueType, long long> || std::is_same_v<ValueType, float> || std::is_same_v<ValueType, double> || std::is_same_v<ValueType, bool>)
                    return static_cast<double>(scalar);
                throw RayTracer::Exception("Expected a numeric scalar value");
            }, value);
        }

        inline std::string stringFromScalar(const RayTracer::ScalarValue &value)
        {
            if (!std::holds_alternative<std::string>(value))
                throw RayTracer::Exception("Expected a string scalar value");
            return std::get<std::string>(value);
        }
    }

    template<typename T>
    inline T fromNode(const RayTracer::NodePtr &node)
    {
        static_assert(sizeof(T) == 0, "fromNode<T>: unsupported type T");
    }

    template<>
    inline double fromNode<double>(const RayTracer::NodePtr &node)
    {
        if (!node || !std::holds_alternative<RayTracer::ScalarValue>(node->value))
            throw RayTracer::Exception("Expected a scalar node");
        return Detail::numericFromScalar(std::get<RayTracer::ScalarValue>(node->value));
    }

    template<>
    inline float fromNode<float>(const RayTracer::NodePtr &node)
    {
        return static_cast<float>(fromNode<double>(node));
    }

    template<>
    inline int fromNode<int>(const RayTracer::NodePtr &node)
    {
        return static_cast<int>(fromNode<double>(node));
    }

    template<>
    inline std::string fromNode<std::string>(const RayTracer::NodePtr &node)
    {
        if (!node || !std::holds_alternative<RayTracer::ScalarValue>(node->value))
            throw RayTracer::Exception("Expected a scalar node");
        return Detail::stringFromScalar(std::get<RayTracer::ScalarValue>(node->value));
    }

    template<>
    inline Geometry::Point3D fromNode<Geometry::Point3D>(const RayTracer::NodePtr &node)
    {
        const RayTracer::Object &object = Detail::asObject(node);
        return Geometry::Point3D(fromNode<float>(Detail::field(object, "x")), fromNode<float>(Detail::field(object, "y")), fromNode<float>(Detail::field(object, "z")));
    }

    template<>
    inline Geometry::Vector3D fromNode<Geometry::Vector3D>(const RayTracer::NodePtr &node)
    {
        const RayTracer::Object &object = Detail::asObject(node);
        return Geometry::Vector3D(fromNode<float>(Detail::field(object, "x")), fromNode<float>(Detail::field(object, "y")), fromNode<float>(Detail::field(object, "z")));
    }

    template<>
    inline RayTracer::Color fromNode<RayTracer::Color>(const RayTracer::NodePtr &node)
    {
        const RayTracer::Object &object = Detail::asObject(node);
        return RayTracer::Color(fromNode<float>(Detail::field(object, "r")) / 255.0, fromNode<float>(Detail::field(object, "g")) / 255.0, fromNode<float>(Detail::field(object, "b")) / 255.0);
    }

    template<>
    inline std::pair<int,int> fromNode<std::pair<int,int>>(const RayTracer::NodePtr &node)
    {
        const RayTracer::Object &object = Detail::asObject(node);
        return std::make_pair(fromNode<int>(Detail::field(object, "width")), fromNode<int>(Detail::field(object, "height")));
    }

    template<>
    inline Geometry::TransformMatrix fromNode<Geometry::TransformMatrix>(const RayTracer::NodePtr &node)
    {
        const RayTracer::Object &object = Detail::asObject(node);
        Geometry::TransformMatrix transform;

        if (object.find("position") != object.end())
        {
            const Geometry::Point3D position = fromNode<Geometry::Point3D>(Detail::field(object, "position"));
            transform = Geometry::TransformMatrix::translation(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z));
        }

        if (object.find("rotation") != object.end())
        {
            const Geometry::Vector3D rotation = fromNode<Geometry::Vector3D>(Detail::field(object, "rotation"));
            transform *= Geometry::TransformMatrix::rotationY(static_cast<float>(rotation.y));
            transform *= Geometry::TransformMatrix::rotationX(static_cast<float>(rotation.x));
            transform *= Geometry::TransformMatrix::rotationZ(static_cast<float>(rotation.z));
        }

        return transform;
    }
}