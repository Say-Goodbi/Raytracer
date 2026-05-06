#include "Camera.hpp"
#include "../../../SceneSystem/Scene/Scene.hpp"
#include "../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <vector>
#include "../../../Utils/Utils.hpp"

namespace RayTracer
{
    /**
     * @brief Generate a ray from the camera through a screen coordinate.
     *
     * Converts normalized screen coordinates (u, v) ∈ [0,1] into a world-space ray
     * by first mapping to the screen rectangle in world space, then computing the
     * ray direction from camera position to screen point.
     *
     * @param u Normalized horizontal screen coordinate (0 = left, 1 = right)
     * @param v Normalized vertical screen coordinate (0 = top, 1 = bottom)
     * @return Ray from camera position with direction toward the screen point
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
     * @brief Cast a ray into the scene and compute the resulting pixel color.
     *
     * Algorithm:
     * 1. Test the ray against all primitives to find the closest intersection
     * 2. If no intersection, return black (0, 0, 0)
     * 3. If intersection found:
     *    a. Check if the hit point is in shadow by casting rays to all lights
     *    b. Compute color using the hit material's shader
     *    c. Darken by 90% if in shadow
     * 4. Clamp color components to [0, 1] range
     *
     * @param ray The ray to cast into the scene
     * @param scene Reference to the scene containing primitives and lights
     * @return Resulting pixel color (R, G, B)
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

        Geometry::Vector3D viewDir = (ray.direction * -1).normalize();

        Geometry::Vector3D bounceDir = mat->sample(closest->normal, viewDir);
        float p = mat->pdf(closest->normal, viewDir, bounceDir);

        if (p < 1e-6f)
            return emitted;

        Geometry::Ray bounceRay(closest->point + closest->normal * 1e-4, bounceDir); // 1e-4 bias to avoid self-intersection
        Color incoming = castRay(bounceRay, scene, depth - 1);
        Color brdf = mat->evaluate(closest->normal, viewDir, bounceDir);
        float cosTheta = std::max(0.0f, (float)closest->normal.dot(bounceDir));
        return emitted + brdf * incoming * (cosTheta / p);
    }

    /**
     * @brief Render the scene to a 2D framebuffer.
     *
     * Iterates over each pixel in the output framebuffer and:
     * 1. Computes normalized screen coordinates (u, v)
     * 2. Generates a ray for that pixel via generateRay()
     * 3. Casts the ray and stores the result in the framebuffer
     *
     * Output format: frame[y][x] where y = row (0 = top), x = column (0 = left)
     *
     * @param scene The scene to render
     * @return 2D vector of colors: frame[height][width]
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
