/**
 * @file src/Objects/Plugins/CorePlugin/Renderers/Camera/Camera.cpp
 * @brief Implementation of the `RayTracer::Camera` renderer.
 *
 * Implements ray generation and deterministic path-tracing for the camera
 * renderer. The camera builds a view plane from a `Geometry::TransformMatrix`
 * and exposes `generateRay()` and `render()` used by the scene writer. See
 * @ref RayTracer::Camera and @ref Geometry::Ray for related types.
 */
#include "Camera.hpp"
#include "../../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <vector>
#include <cmath>
#include <mutex>
#include "../../../../../Utils/FrameBufferMutex.hpp"
#include "../../../../../Utils/Utils.hpp"

namespace RayTracer
{
    Camera::Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width, int height)
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

    Camera::Camera(Geometry::TransformMatrix transform, float fov, int width, int height)
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
    Geometry::Ray Camera::generateRay(double u, double v)
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

    void Camera::buildScreenFromTransform()
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
     * @brief Evaluate radiance along a ray using a two-branch shading model.
     *
     * The branch is chosen by the material's PDF for its sampled direction:
     *
     * **Specular / glass branch** (`pdf ≈ 0`, delta BSDFs):
     * @code
     *   L = Le + evaluate(ωi, ωo) * π * castRay(ωi)
     * @endcode
     * The bounce origin is offset by 1e-4 along `ωi` to avoid self-intersection.
     *
     * **Diffuse branch** (`pdf > 0`):
     * @code
     *   L = Le + evaluate(n, ωo, n) * π * Σ light->computeLight(hit)
     * @endcode
     * Incoming radiance comes from direct lighting only — no recursive bounce.
     *
     * Steps:
     * 1. Return black if the bounce budget is exhausted.
     * 2. Find the nearest primitive intersection; return black on miss.
     * 3. Query the material for emitted radiance (`Le`) and a bounce direction.
     * 4. Compute `pdf` for that direction.
     * 5. If `pdf < 1e-6` (specular/glass): recurse along the bounce direction.
     * 6. Otherwise: accumulate direct light from every scene light.
     * 7. Combine emitted + shaded term and return.
     *
     * @param ray   Incoming ray to evaluate.
     * @param scene Scene containing all primitives and lights.
     * @param depth Remaining bounce budget; returns black when ≤ 0.
     * @return Radiance estimate along @p ray.
     */
    Color Camera::castRay(const Geometry::Ray &ray, Scene &scene, int depth) {
        if (depth <= 0)
            return Color(0, 0, 0);
        std::optional<Geometry::HitRecord> closest = scene.hit(ray);

        if (!closest)
            return Color(0, 0, 0); // No intersection: return black (AKA: background, maybe change to a blue background ?)

        IMaterial *mat = closest->material;
        Color emitted = mat->emission();

        Geometry::Vector3D viewDir = (ray.direction * -1).normalize();
        Geometry::Vector3D bounceDir = mat->sample(closest->normal, viewDir);
        float p = mat->pdf(closest->normal, viewDir, bounceDir);

        if (p < 1e-6f) {
            // Specular / glass: pure recursive bounce, no direct lighting
            Geometry::Point3D bounceOrigin(
                closest->point.x + bounceDir.x * 1e-4f,
                closest->point.y + bounceDir.y * 1e-4f,
                closest->point.z + bounceDir.z * 1e-4f);
            Color tint = mat->evaluate(closest->normal, viewDir, bounceDir) * M_PI;
            return emitted + tint * castRay(Geometry::Ray(bounceOrigin, bounceDir), scene, depth - 1);
        }

        Color directLight(0, 0, 0);
        for (const auto& light : scene.getLights())
            directLight = directLight + light->computeLight(*closest, scene.getPrimitives());

        Color albedo = mat->evaluate(closest->normal, viewDir, closest->normal) * M_PI;
        return emitted + albedo * directLight;
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
    std::vector<std::vector<Color>> Camera::render(Scene& scene, std::vector<std::vector<Color>>& framebuffer) {
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

        for (int y = 0; y < _height; y++) {
            std::vector<Color> row(_width, Color(0, 0, 0));
            for (int x = 0; x < _width; x++) {
                Color accumulated(0, 0, 0);

                for (int sy = 0; sy < _sqrtSamples; sy++) {
                    for (int sx = 0; sx < _sqrtSamples; sx++) {
                        double u = (x + (sx + 0.5) / _sqrtSamples) / (_width - 1);
                        double v = (y + (sy + 0.5) / _sqrtSamples) / (_height - 1);
                        Geometry::Ray ray = generateRay(u, v);
                        accumulated = accumulated + castRay(ray, scene, _maxDepth);
                    }
                }

                int totalSamples = _sqrtSamples * _sqrtSamples;
                row[x] = (accumulated * (1.0 / totalSamples)).clamp();
            }

            {
                std::unique_lock<std::shared_mutex> lock(FrameBufferMutex());
                framebuffer[y] = std::move(row);
            }
        }
        return framebuffer;
    }
} // namespace RayTracer
