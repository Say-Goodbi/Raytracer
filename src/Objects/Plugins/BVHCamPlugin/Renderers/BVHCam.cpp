/**
 * @file src/Objects/Plugins/CorePlugin/Renderers/Camera/Camera.cpp
 * @brief Implementation of the `RayTracer::BVHCam` renderer.
 *
 * Implements ray generation and deterministic path-tracing for the camera
 * renderer. The camera builds a view plane from a `Geometry::TransformMatrix`
 * and exposes `generateRay()` and `render()` used by the scene writer. See
 * @ref RayTracer::BVHCam and @ref Geometry::Ray for related types.
 */
#include "BVHCam.hpp"
#include "../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <vector>
#include <cmath>
#include <mutex>
#include <algorithm>
#include "../../../../Utils/FrameBufferMutex.hpp"
#include "../../../../Utils/Utils.hpp"

namespace RayTracer
{
    namespace
    {
        struct TraversalStats
        {
            std::size_t nodeChecks = 0;
            std::size_t primitiveChecks = 0;
        };

        Color lerpColor(const Color &a, const Color &b, double t)
        {
            return Color(
                a.r + (b.r - a.r) * t,
                a.g + (b.g - a.g) * t,
                a.b + (b.b - a.b) * t);
        }

        Color checksToHeatColor(double normalized)
        {
            const double t = std::clamp(normalized, 0.0, 1.0);

            // Use a longer gray ramp before transitioning into colors.
            // First part: black -> dark gray -> light gray (grayThreshold portion)
            // Second part: color ramp (remaining portion)
            const double grayThreshold = 0.6; // 60% of the ramp reserved for grays

            if (t <= 0.0)
                return Color(0.0, 0.0, 0.0);

            if (t < grayThreshold) {
                // interpolate from black -> dark gray -> light gray across [0, grayThreshold]
                double gt = t / grayThreshold; // normalized within gray segment [0,1]
                if (gt < 0.5) {
                    // black -> dark gray (first half)
                    double localT = gt / 0.5;
                    return lerpColor(Color(0.0,0.0,0.0), Color(0.2,0.2,0.2), localT);
                } else {
                    // dark gray -> light gray (second half)
                    double localT = (gt - 0.5) / 0.5;
                    return lerpColor(Color(0.2,0.2,0.2), Color(0.85,0.85,0.85), localT);
                }
            }

            // Color ramp portion mapped from [grayThreshold, 1.0] -> [0,1]
            double ct = (t - grayThreshold) / (1.0 - grayThreshold);

            const Color colorStops[] = {
                Color(1.0, 1.0, 0.0),   // yellow
                Color(1.0, 0.5, 0.0),   // orange
                Color(1.0, 0.0, 0.0),   // red
                Color(0.6, 0.0, 1.0),   // violet
                Color(0.0, 0.0, 1.0)    // blue
            };
            const int colorSegments = 4; // stops-1
            double scaled = ct * colorSegments;
            int idx = static_cast<int>(std::floor(scaled));
            if (idx < 0) idx = 0;
            if (idx >= colorSegments) idx = colorSegments - 1;
            double localT = scaled - idx;
            return lerpColor(colorStops[idx], colorStops[idx + 1], localT);
        }

        std::optional<Geometry::HitRecord> traverseNodeWithStats(const BVH::Node *node, const Geometry::Ray &ray, TraversalStats &stats)
        {
            if (!node)
                return std::nullopt;

            ++stats.nodeChecks;
            if (!node->bounds.hit(ray))
                return std::nullopt;

            std::optional<Geometry::HitRecord> closest;

            if (node->isLeaf())
            {
                for (const auto &primitive : node->primitives)
                {
                    ++stats.primitiveChecks;
                    std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
                    if (hit && (!closest || hit->rayDistance < closest->rayDistance))
                        closest = hit;
                }
                return closest;
            }

            std::optional<Geometry::HitRecord> leftHit = traverseNodeWithStats(node->left.get(), ray, stats);
            std::optional<Geometry::HitRecord> rightHit = traverseNodeWithStats(node->right.get(), ray, stats);

            if (leftHit && rightHit)
                return leftHit->rayDistance < rightHit->rayDistance ? leftHit : rightHit;
            return leftHit ? leftHit : rightHit;
        }

        std::optional<Geometry::HitRecord> traverseSceneWithStats(Scene &scene, const Geometry::Ray &ray, TraversalStats &stats)
        {
            std::optional<Geometry::HitRecord> closest;

            std::optional<std::reference_wrapper<const BVH>> bvhRef = scene.getBVH();
            if (bvhRef)
                closest = traverseNodeWithStats(bvhRef->get().root(), ray, stats);

            for (const auto &primitive : scene.getPrimitives())
            {
                if (primitive->getBounds())
                    continue;

                ++stats.primitiveChecks;
                std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
                if (hit && (!closest || hit->rayDistance < closest->rayDistance))
                    closest = hit;
            }

            return closest;
        }
    }

    BVHCam::BVHCam(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width, int height)
        : ARenderer(width, height), _fov(fov)
    {
        // Build a transform matrix from position + forward direction.
        Geometry::Vector3D forward = (direction.length() > 1e-6)
            ? direction.normalize()
            : Geometry::Vector3D(0, 1, 0); // default: look toward +y

        Geometry::Vector3D worldUp = (std::abs(forward.z) > 0.9)
            ? Geometry::Vector3D(0, 1, 0)
            : Geometry::Vector3D(0, 0, 1); // z-up unless looking straight up/down

        Geometry::Vector3D right     = Geometry::Vector3D::cross(forward, worldUp).normalize();
        Geometry::Vector3D screenUp  = Geometry::Vector3D::cross(right, forward).normalize();

        Geometry::TransformMatrix t = Geometry::TransformMatrix::identity();
        // columns: right, up, forward
        t(0,0) = right.x;  t(1,0) = right.y;  t(2,0) = right.z;
        t(0,1) = screenUp.x; t(1,1) = screenUp.y; t(2,1) = screenUp.z;
        t(0,2) = forward.x; t(1,2) = forward.y; t(2,2) = forward.z;
        // translation
        t(0,3) = pos.x; t(1,3) = pos.y; t(2,3) = pos.z;

        _transform = t;
        buildScreenFromTransform();
    }

    BVHCam::BVHCam(Geometry::TransformMatrix transform, float fov, int width, int height)
        : ARenderer(width, height), _transform(transform), _fov(fov)
    {
        buildScreenFromTransform();
    }

    /**
     * @brief Build a world-space ray from normalized screen coordinates.
     *
     * Maps (u, v) ∈ [0, 1]² onto the screen rectangle in world space, then
     * returns a ray from the camera position toward that point.
     *
     * @param u Horizontal position (0 = left, 1 = right).
     * @param v Vertical position (0 = top, 1 = bottom).
     * @return Unit-direction ray from the camera through the screen point.
     */
    Geometry::Ray BVHCam::generateRay(double u, double v)
    {
        Geometry::Point3D screenPoint = this->_screen.pointAt(u, v);
        Geometry::Point3D origin = this->_transform * Geometry::Point3D(0, 0, 0);
        Geometry::Vector3D rayDirection = Geometry::Vector3D(
                                              screenPoint.x - origin.x,
                                              screenPoint.y - origin.y,
                                              screenPoint.z - origin.z)
                                              .normalize();
        return Geometry::Ray(origin, rayDirection);
    }

    void BVHCam::buildScreenFromTransform()
    {
        // Extract basis columns and translation from _transform
        Geometry::Point3D pos(
            _transform(0,3),
            _transform(1,3),
            _transform(2,3));

        Geometry::Vector3D right(
            _transform(0,0),
            _transform(1,0),
            _transform(2,0));

        Geometry::Vector3D up(
            _transform(0,1),
            _transform(1,1),
            _transform(2,1));

        Geometry::Vector3D forward(
            _transform(0,2),
            _transform(1,2),
            _transform(2,2));

        const double rightScale = std::max(right.length(), 1e-6);
        const double upScale = std::max(up.length(), 1e-6);

        // Normalize basis vectors (in case transform wasn't orthonormal)
        forward = (forward.length() > 1e-6) ? forward.normalize() : Geometry::Vector3D(0, 1, 0);
        right = (right.length() > 1e-6) ? right.normalize() : Geometry::Vector3D::cross(forward, Geometry::Vector3D(0,0,1)).normalize();
        up = Geometry::Vector3D::cross(right, forward).normalize();

        double fovRad = _fov * M_PI / 180.0;
        double halfW  = std::tan(fovRad / 2.0) * rightScale;
        double halfH  = std::tan(fovRad / 2.0) * ((double)_height / _width) * upScale;

        // Screen center is 1 unit in front of the camera
        Geometry::Point3D center(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z);

        // Top-left corner: move left by halfW and up by halfH
        _screen.origin = Geometry::Point3D(
            center.x - right.x * halfW + up.x * halfH,
            center.y - right.y * halfW + up.y * halfH,
            center.z - right.z * halfW + up.z * halfH);

        _screen.width = Geometry::Vector3D(right.x * 2*halfW, right.y * 2*halfW, right.z * 2*halfW);

        _screen.height = Geometry::Vector3D(-up.x * 2*halfH, -up.y * 2*halfH, -up.z * 2*halfH);
    }

    /**
     * @brief Render the scene into a 2D framebuffer.
     *
     * Each pixel is super-sampled with a deterministic stratified grid of
     * `_sqrtSamples × _sqrtSamples` sub-pixel rays.  Sample positions are
     * fixed at the **center of each sub-cell** — no jitter, no random offset —
     * keeping the overall render fully deterministic.
     *
     * Per-pixel steps:
     * 1. Iterate over the `_sqrtSamples × _sqrtSamples` sub-cell grid.
     * 2. Compute the sub-cell center in normalized screen space (u, v).
     * 3. Generate and path-trace a ray via `generateRay()` + `castRay()`.
     * 4. Average all sub-pixel radiance values and clamp to [0, 1].
     *
     * Output layout: `frame[y][x]`, y = 0 at the top, x = 0 at the left.
     *
     * @param scene Scene to render.
     * @return 2D grid of clamped colors: `frame[height][width]`.
     */
    std::vector<std::vector<Color>> BVHCam::render(Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
        scene.prepareAccelerationStructure();

        {
            std::unique_lock<std::shared_mutex> lock(FrameBufferMutex());
            // Ensure framebuffer is the correct size before incremental updates start.
            if ((int)framebuffer.size() != _height) framebuffer.assign(_height, std::vector<Color>(_width));
            else {
                for (auto &row : framebuffer) {
                    if ((int)row.size() != _width) row.assign(_width, Color(0,0,0));
                }
            }
        }

        std::vector<double> checksPerPixel(static_cast<std::size_t>(_width) * static_cast<std::size_t>(_height), 0.0);
        double maxChecks = 0.0;
        double minChecks = std::numeric_limits<double>::infinity();

        for (int y = 0; y < _height; y++) {
            std::vector<Color> row(_width, Color(0, 0, 0));
            for (int x = 0; x < _width; x++) {
                double accumulatedChecks = 0.0;

                for (int sy = 0; sy < _sqrtSamples; sy++) {
                    for (int sx = 0; sx < _sqrtSamples; sx++) {
                        double u = (x + (sx + 0.5) / _sqrtSamples) / (_width - 1);
                        double v = (y + (sy + 0.5) / _sqrtSamples) / (_height - 1);
                        Geometry::Ray ray = generateRay(u, v);

                        TraversalStats stats;
                        (void)traverseSceneWithStats(scene, ray, stats);
                        accumulatedChecks += static_cast<double>(stats.nodeChecks + stats.primitiveChecks);
                    }
                }

                int totalSamples = _sqrtSamples * _sqrtSamples;
                const double averageChecks = accumulatedChecks / static_cast<double>(totalSamples);
                checksPerPixel[static_cast<std::size_t>(y) * static_cast<std::size_t>(_width) + static_cast<std::size_t>(x)] = averageChecks;
                maxChecks = std::max(maxChecks, averageChecks);
                minChecks = std::min(minChecks, averageChecks);
                row[x] = Color(0, 0, 0);
            }

            {
                std::unique_lock<std::shared_mutex> lock(FrameBufferMutex());
                framebuffer[y] = std::move(row);
            }
        }

        for (int y = 0; y < _height; ++y) {
            std::vector<Color> row(_width, Color(0, 0, 0));
            for (int x = 0; x < _width; ++x) {
                const double checks = checksPerPixel[static_cast<std::size_t>(y) * static_cast<std::size_t>(_width) + static_cast<std::size_t>(x)];
                double normalized = 0.0;
                // Normalize across observed range. If all pixels have the same
                // count (max==min) fall back to checks/maxChecks if possible,
                // otherwise zero to show black.
                if (maxChecks > minChecks + 1e-12) {
                    normalized = (checks - minChecks) / (maxChecks - minChecks);
                } else if (maxChecks > 1e-12) {
                    normalized = checks / maxChecks;
                } else {
                    normalized = 0.0;
                }
                row[x] = checksToHeatColor(normalized).clamp();
            }

            std::unique_lock<std::shared_mutex> lock(FrameBufferMutex());
            framebuffer[y] = std::move(row);
        }

        return framebuffer;
    }
} // namespace RayTracer
