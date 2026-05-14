#pragma once

#include <algorithm>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include "../../Geometry/AABB/AABB.hpp"
#include "../../Objects/Abstracts/APrimitive/APrimitive.hpp"

namespace RayTracer
{
    class BVH
    {
    private:
        struct Record
        {
            std::shared_ptr<APrimitive> primitive;
            Geometry::AABB bounds;
            Geometry::Point3D centroid;
        };

        struct Node
        {
            Geometry::AABB bounds;
            std::unique_ptr<Node> left;
            std::unique_ptr<Node> right;
            std::vector<std::shared_ptr<APrimitive>> primitives;

            bool isLeaf() const
            {
                return !left && !right;
            }
        };

        std::unique_ptr<Node> _root;

        static std::optional<Geometry::AABB> primitiveBounds(const std::shared_ptr<APrimitive> &primitive)
        {
            return primitive ? primitive->getBounds() : std::nullopt;
        }

        static Geometry::AABB boundsFromRecords(const std::vector<Record> &records)
        {
            Geometry::AABB bounds = records.front().bounds;
            for (std::size_t index = 1; index < records.size(); ++index)
                bounds = Geometry::AABB::unite(bounds, records[index].bounds);
            return bounds;
        }

        static std::unique_ptr<Node> build(std::vector<Record> records)
        {
            if (records.empty())
                return nullptr;

            auto node = std::make_unique<Node>();
            node->bounds = boundsFromRecords(records);

            if (records.size() <= 2)
            {
                for (const Record &record : records)
                    node->primitives.push_back(record.primitive);
                return node;
            }

            const Geometry::AABB centroidBounds(
                Geometry::Point3D(
                    records.front().centroid.x,
                    records.front().centroid.y,
                    records.front().centroid.z),
                Geometry::Point3D(
                    records.front().centroid.x,
                    records.front().centroid.y,
                    records.front().centroid.z));

            Geometry::AABB centroidRange = centroidBounds;
            for (std::size_t index = 1; index < records.size(); ++index)
            {
                const Geometry::Point3D centroidPoint(records[index].centroid.x, records[index].centroid.y, records[index].centroid.z);
                centroidRange = Geometry::AABB::unite(centroidRange, Geometry::AABB(centroidPoint, centroidPoint));
            }

            const double rangeX = centroidRange.maximum.x - centroidRange.minimum.x;
            const double rangeY = centroidRange.maximum.y - centroidRange.minimum.y;
            const double rangeZ = centroidRange.maximum.z - centroidRange.minimum.z;
            const int axis = (rangeY > rangeX && rangeY >= rangeZ) ? 1 : ((rangeZ > rangeX && rangeZ >= rangeY) ? 2 : 0);

            auto bestSplit = records.end();
            double bestCost = static_cast<double>(records.size());

            auto evaluateAxis = [&](int candidateAxis)
            {
                std::sort(records.begin(), records.end(), [candidateAxis](const Record &lhs, const Record &rhs) {
                    const double lhsValue = candidateAxis == 0 ? lhs.centroid.x : candidateAxis == 1 ? lhs.centroid.y : lhs.centroid.z;
                    const double rhsValue = candidateAxis == 0 ? rhs.centroid.x : candidateAxis == 1 ? rhs.centroid.y : rhs.centroid.z;
                    return lhsValue < rhsValue;
                });

                std::vector<Geometry::AABB> prefix(records.size());
                std::vector<Geometry::AABB> suffix(records.size());
                prefix[0] = records[0].bounds;
                for (std::size_t index = 1; index < records.size(); ++index)
                    prefix[index] = Geometry::AABB::unite(prefix[index - 1], records[index].bounds);
                suffix.back() = records.back().bounds;
                for (std::size_t index = records.size() - 1; index-- > 0;)
                    suffix[index] = Geometry::AABB::unite(suffix[index + 1], records[index].bounds);

                const double parentArea = node->bounds.surfaceArea();
                if (parentArea <= 0.0)
                    return;

                for (std::size_t split = 1; split < records.size(); ++split)
                {
                    const double leftArea = prefix[split - 1].surfaceArea();
                    const double rightArea = suffix[split].surfaceArea();
                    const double cost = 1.0 + (leftArea / parentArea) * static_cast<double>(split) + (rightArea / parentArea) * static_cast<double>(records.size() - split);
                    if (cost < bestCost)
                    {
                        bestCost = cost;
                        bestSplit = records.begin() + static_cast<std::ptrdiff_t>(split);
                    }
                }
            };

            evaluateAxis(axis);
            evaluateAxis((axis + 1) % 3);
            evaluateAxis((axis + 2) % 3);

            if (bestSplit == records.end())
            {
                for (const Record &record : records)
                    node->primitives.push_back(record.primitive);
                return node;
            }

            std::vector<Record> leftRecords(records.begin(), bestSplit);
            std::vector<Record> rightRecords(bestSplit, records.end());

            if (leftRecords.empty() || rightRecords.empty())
            {
                for (const Record &record : records)
                    node->primitives.push_back(record.primitive);
                return node;
            }

            node->left = build(std::move(leftRecords));
            node->right = build(std::move(rightRecords));
            if (!node->left || !node->right)
            {
                node->left.reset();
                node->right.reset();
                for (const Record &record : records)
                    node->primitives.push_back(record.primitive);
            }
            return node;
        }

        static std::optional<Geometry::HitRecord> hitNode(const Node *node, const Geometry::Ray &ray)
        {
            if (!node || !node->bounds.hit(ray))
                return std::nullopt;

            std::optional<Geometry::HitRecord> closest;

            if (node->isLeaf())
            {
                for (const auto &primitive : node->primitives)
                {
                    std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
                    if (hit && (!closest || hit->rayDistance < closest->rayDistance))
                        closest = hit;
                }
                return closest;
            }

            std::optional<Geometry::HitRecord> leftHit = hitNode(node->left.get(), ray);
            std::optional<Geometry::HitRecord> rightHit = hitNode(node->right.get(), ray);

            if (leftHit && rightHit)
                return leftHit->rayDistance < rightHit->rayDistance ? leftHit : rightHit;
            return leftHit ? leftHit : rightHit;
        }

    public:
        BVH() = default;

        explicit BVH(const std::vector<std::shared_ptr<APrimitive>> &primitives)
        {
            std::vector<Record> records;
            records.reserve(primitives.size());

            for (const auto &primitive : primitives)
            {
                std::optional<Geometry::AABB> bounds = primitiveBounds(primitive);
                if (!bounds)
                    continue;

                records.push_back({primitive, *bounds, bounds->centroid()});
            }

            _root = build(std::move(records));
        }

        bool empty() const
        {
            return !_root;
        }

        std::optional<Geometry::HitRecord> hit(const Geometry::Ray &ray) const
        {
            return hitNode(_root.get(), ray);
        }
    };
}