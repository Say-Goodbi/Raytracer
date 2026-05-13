#include "Camera.hpp"
#include "../../../../../SceneSystem/Scene/Scene.hpp"
#include "../../../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <vector>
#include <cmath>
#include "../../../../../Utils/Utils.hpp"

namespace RayTracer
{
    Camera::Camera(Geometry::Point3D pos, Geometry::Vector3D direction, float fov, int width, int height)
        : ARenderer(width, height), _position(pos), _direction(direction), _fov(fov)
    {
        Geometry::Vector3D forward = (direction.length() > 1e-6)
            ? direction.normalize()
            : Geometry::Vector3D(0, 1, 0); // default: look toward +y

        Geometry::Vector3D worldUp = (std::abs(forward.z) > 0.9)
            ? Geometry::Vector3D(0, 1, 0)
            : Geometry::Vector3D(0, 0, 1); // z-up unless looking straight up/down

        Geometry::Vector3D right     = Geometry::Vector3D::cross(forward, worldUp).normalize();
        Geometry::Vector3D screenUp  = Geometry::Vector3D::cross(right, forward).normalize();

        double fovRad = fov * M_PI / 180.0;
        double halfW  = std::tan(fovRad / 2.0);
        double halfH  = halfW * ((double)height / width);

        Geometry::Point3D center(pos.x + forward.x, pos.y + forward.y, pos.z + forward.z);

        _screen.origin = Geometry::Point3D(
            center.x - right.x * halfW + screenUp.x * halfH,
            center.y - right.y * halfW + screenUp.y * halfH,
            center.z - right.z * halfW + screenUp.z * halfH);

        _screen.width = Geometry::Vector3D(right.x * 2*halfW, right.y * 2*halfW, right.z * 2*halfW);

        _screen.height = Geometry::Vector3D(-screenUp.x * 2*halfH, -screenUp.y * 2*halfH, -screenUp.z * 2*halfH);
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
        Geometry::Vector3D rayDirection = Geometry::Vector3D(
                                              screenPoint.x - this->_position.x,
                                              screenPoint.y - this->_position.y,
                                              screenPoint.z - this->_position.z)
                                              .normalize();
        return Geometry::Ray(this->_position, rayDirection);
    }

    /**
     * @brief Evaluate the rendering equation along a ray, deterministically.
     *
     * Implements the standard Monte Carlo path-tracing estimator:
     * @code
     *   L(x, ωo) = Le(x, ωo) + ∫ f(x, ωi, ωo) Li(x, ωi) cosθi dωi
     * @endcode
     * but evaluates the integral at a **single deterministic direction** supplied
     * by `IMaterial::sample()` instead of drawing a random sample.  The
     * estimator is then:
     * @code
     *   L ≈ Le + f(ωi, ωo) * Li(ωi) * cosθi / pdf(ωi)
     * @endcode
     * This removes noise and makes rendering fully reproducible at the cost of
     * variance reduction that random multi-sampling would otherwise provide.
     *
     * Steps:
     * 1. Return black if the bounce budget is exhausted.
     * 2. Find the nearest primitive intersection; return black on miss.
     * 3. Query the material for its emitted radiance (`Le`).
     * 4. Ask the material for a deterministic bounce direction (`ωi`).
     * 5. If `pdf(ωi) ≈ 0`, return only the emitted term (degenerate surface).
     * 6. Recurse along `ωi` (with a small normal-bias offset to avoid
     *    self-intersection) to get incoming radiance `Li`.
     * 7. Combine: `Le + brdf * Li * cosθ / pdf`.
     *
     * @param ray   Incoming ray to evaluate.
     * @param scene Scene containing all primitives.
     * @param depth Remaining bounce budget; returns black when ≤ 0.
     * @return Radiance estimate along @p ray.
     */
    Color Camera::castRay(const Geometry::Ray &ray, Scene &scene, int depth) {
        if (depth <= 0)
            return Color(0, 0, 0);
        std::optional<Geometry::HitRecord> closest;

        for (const auto &primitive : scene.getPrimitives()) {
            std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
            if (hit && (!closest || hit->rayDistance < closest->rayDistance))
                closest = hit;
        }
        if (!closest)
            return Color(0, 0, 0); // No intersection: return black (AKA: background, maybe change to a blue background ?)

        IMaterial *mat = closest->material;
        Color emitted = mat->emission();

        Color directLight(0, 0, 0);
        for (const auto& light : scene.getLights())
            directLight = directLight + light->computeLight(*closest, scene.getPrimitives());

        Geometry::Vector3D viewDir = (ray.direction * -1).normalize();
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
    std::vector<std::vector<Color>> Camera::render(Scene& scene) {
        std::vector<std::vector<Color>> framebuffer(_height, std::vector<Color>(_width));

        for (int y = 0; y < _height; y++) {
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
                framebuffer[y][x] = (accumulated * (1.0 / totalSamples)).clamp();
            }
        }
        return framebuffer;
    }
} // namespace RayTracer
