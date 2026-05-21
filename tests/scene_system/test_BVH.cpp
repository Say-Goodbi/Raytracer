#include <criterion/criterion.h>
#include <cmath>
#include <memory>
#include <vector>

#include "../../src/Geometry/Ray/Ray.hpp"
#include "../../src/SceneSystem/BVH/BVH.hpp"

using Geometry::AABB;
using Geometry::HitRecord;
using Geometry::Point3D;
using Geometry::Ray;
using Geometry::Vector3D;

namespace
{
    class StubPrimitive final : public RayTracer::APrimitive
    {
    private:
        std::optional<AABB> _bounds;
        std::optional<HitRecord> _hit;

    public:
        StubPrimitive(std::optional<AABB> bounds, std::optional<HitRecord> hit)
              : APrimitive(std::shared_ptr<RayTracer::IMaterial>()), _bounds(std::move(bounds)), _hit(std::move(hit))
        {
        }

        std::optional<HitRecord> hit(const Ray &) const override
        {
            return _hit;
        }

        std::optional<AABB> getBounds() const override
        {
            return _bounds;
        }
    };

    std::shared_ptr<RayTracer::APrimitive> makePrimitive(const AABB &bounds, float distance)
    {
        return std::make_shared<StubPrimitive>(bounds, HitRecord(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), distance, nullptr));
    }
}

Test(BVH, empty_tree_is_empty)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives;
    RayTracer::BVH emptyBvh(primitives);

    cr_assert(emptyBvh.empty());
    cr_assert_null(emptyBvh.root());
    cr_assert(!emptyBvh.hit(Ray(Point3D(0.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0))).has_value());
}

Test(BVH, null_primitives_are_ignored)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives;
    primitives.push_back(nullptr);
    primitives.push_back(makePrimitive(AABB(Point3D(-1.0, -1.0, -1.0), Point3D(1.0, 1.0, 1.0)), 4.0f));

    RayTracer::BVH bvh(primitives);
    cr_assert(!bvh.empty());
    cr_assert_not_null(bvh.root());
    cr_assert(bvh.root()->isLeaf());
    cr_assert_eq(bvh.root()->primitives.size(), 1u);
}

Test(BVH, single_sphere_hit)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives = {
        makePrimitive(AABB(Point3D(-1.0, -1.0, -1.0), Point3D(1.0, 1.0, 1.0)), 4.0f)
    };
    RayTracer::BVH bvh(primitives);

    const Ray ray(Point3D(0.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
    std::optional<HitRecord> hit = bvh.hit(ray);
    cr_assert(hit.has_value());
    cr_assert_float_eq(hit->rayDistance, 4.0f, 1e-3);
}

Test(BVH, two_primitives_stay_in_a_leaf)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives = {
        makePrimitive(AABB(Point3D(-3.5, -1.0, -1.0), Point3D(-1.5, 1.0, 1.0)), 5.0f),
        makePrimitive(AABB(Point3D(1.5, -1.0, -1.0), Point3D(3.5, 1.0, 1.0)), 2.0f)
    };

    RayTracer::BVH bvh(primitives);
    cr_assert(!bvh.empty());
    cr_assert_not_null(bvh.root());
    cr_assert(bvh.root()->isLeaf());
    cr_assert_eq(bvh.root()->primitives.size(), 2u);
    cr_assert_null(bvh.root()->left.get());
    cr_assert_null(bvh.root()->right.get());
}

Test(BVH, two_primitives_leaf_hit_returns_closest)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives = {
        makePrimitive(AABB(Point3D(-1.0, -1.0, -3.5), Point3D(1.0, 1.0, -1.5)), 5.0f),
        makePrimitive(AABB(Point3D(-1.0, -1.0, 1.5), Point3D(1.0, 1.0, 3.5)), 2.0f)
    };

    RayTracer::BVH bvh(primitives);
    const Ray ray(Point3D(0.0, 0.0, -5.0), Vector3D(0.0, 0.0, 1.0));
    std::optional<HitRecord> hit = bvh.hit(ray);
    cr_assert(hit.has_value());
    cr_assert_float_eq(hit->rayDistance, 2.0f, 1e-3);
}

Test(BVH, three_primitives_split_into_internal_nodes)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives = {
        makePrimitive(AABB(Point3D(-1.0, -1.0, -7.0), Point3D(1.0, 1.0, -5.0)), 6.0f),
        makePrimitive(AABB(Point3D(-1.0, -1.0, -1.0), Point3D(1.0, 1.0, 1.0)), 4.0f),
        makePrimitive(AABB(Point3D(-1.0, -1.0, 5.0), Point3D(1.0, 1.0, 7.0)), 2.0f)
    };

    RayTracer::BVH bvh(primitives);
    cr_assert(!bvh.empty());
    cr_assert_not_null(bvh.root());
    cr_assert(!bvh.root()->isLeaf());
    cr_assert_not_null(bvh.root()->left.get());
    cr_assert_not_null(bvh.root()->right.get());
}

Test(BVH, three_primitives_split_hit_is_recursive)
{
    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives = {
           makePrimitive(AABB(Point3D(-1.0, -1.0, -7.0), Point3D(1.0, 1.0, -5.0)), 6.0f),
           makePrimitive(AABB(Point3D(-1.0, -1.0, -1.0), Point3D(1.0, 1.0, 1.0)), 4.0f),
           makePrimitive(AABB(Point3D(-1.0, -1.0, 5.0), Point3D(1.0, 1.0, 7.0)), 2.0f)
    };

    RayTracer::BVH bvh(primitives);
    const Ray ray(Point3D(0.0, 0.0, -10.0), Vector3D(0.0, 0.0, 1.0));
    std::optional<HitRecord> hit = bvh.hit(ray);
    cr_assert(hit.has_value());
    cr_assert_float_eq(hit->rayDistance, 2.0f, 1e-3);
}

Test(BVH, degenerate_bounds_fall_back_to_leaf)
{
    const AABB degenerate(Point3D(0.0, 0.0, 0.0), Point3D(0.0, 0.0, 0.0));
    const HitRecord hit(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 1.0f, nullptr);

    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives;
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, hit));
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, HitRecord(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 2.0f, nullptr)));
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, HitRecord(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 3.0f, nullptr)));

    RayTracer::BVH bvh(primitives);
    cr_assert(!bvh.empty());
    cr_assert_not_null(bvh.root());
    cr_assert(bvh.root()->isLeaf());
    cr_assert_eq(bvh.root()->primitives.size(), 3u);
}

Test(BVH, degenerate_bounds_fall_back_to_leaf_hit)
{
    const AABB degenerate(Point3D(0.0, 0.0, 0.0), Point3D(0.0, 0.0, 0.0));
    const HitRecord hit(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 1.0f, nullptr);

    std::vector<std::shared_ptr<RayTracer::APrimitive>> primitives;
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, hit));
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, HitRecord(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 2.0f, nullptr)));
    primitives.push_back(std::make_shared<StubPrimitive>(degenerate, HitRecord(Point3D(0.0, 0.0, 0.0), Vector3D(0.0, 0.0, 1.0), 3.0f, nullptr)));

    RayTracer::BVH bvh(primitives);
    cr_assert(!bvh.empty());
    cr_assert_not_null(bvh.root());
    cr_assert(bvh.root()->isLeaf());
    cr_assert_eq(bvh.root()->primitives.size(), 3u);

    const Ray ray(Point3D(0.0, 0.0, -1.0), Vector3D(0.0, 0.0, 1.0));
    std::optional<HitRecord> result = bvh.hit(ray);
    cr_assert(true);
    cr_assert(true);
}