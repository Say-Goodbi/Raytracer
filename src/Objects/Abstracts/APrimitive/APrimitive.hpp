#pragma once
#include "../IMaterial.hpp"
#include <memory>
#include <optional>
#include "../../../Geometry/AABB/AABB.hpp"
#include "../../../Geometry/Ray/Ray.hpp"

namespace RayTracer
{
    /// Abstract base class for all primitives (renderable objects).
    /// Primitives have geometry and a material.
    class APrimitive
    {
    private:
        std::shared_ptr<IMaterial> _material;  ///< Material defining shading behavior

    public:
        /// Constructor.
        /// @param material The material for this primitive
        APrimitive(std::shared_ptr<IMaterial> material) : _material(std::move(material)) {};
        virtual ~APrimitive() = default;

        /// Test ray intersection with this primitive.
        /// @param ray The ray to test
        /// @return Optional HitRecord if intersection occurs, empty otherwise
        virtual std::optional<Geometry::HitRecord> hit(const Geometry::Ray& ray) const = 0;

        /// Get the primitive's axis-aligned bounds, if any.
        /// Unbounded primitives may return std::nullopt.
        virtual std::optional<Geometry::AABB> getBounds() const = 0;

        /// Get the material of this primitive.
        /// @return Pointer to the material
        IMaterial *getMaterial() const;
    };
} // namespace RayTracer
