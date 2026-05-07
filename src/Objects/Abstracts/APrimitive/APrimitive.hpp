#pragma once
#include "../IMaterial.hpp"
#include <optional>

namespace Geometry { class Ray; }

namespace RayTracer
{
    /// Abstract base class for all primitives (renderable objects).
    /// Primitives have geometry and a material.
    class APrimitive
    {
    private:
        IMaterial *_material;  ///< Material defining shading behavior

    public:
        /// Constructor.
        /// @param material The material for this primitive
        APrimitive(IMaterial *material) : _material(material) {};
        virtual ~APrimitive() = default;

        /// Test ray intersection with this primitive.
        /// @param ray The ray to test
        /// @return Optional HitRecord if intersection occurs, empty otherwise
        virtual std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const = 0;

        /// Get the material of this primitive.
        /// @return Pointer to the material
        IMaterial *getMaterial() const;
    };
} // namespace RayTracer
