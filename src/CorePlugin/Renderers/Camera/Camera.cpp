#include "Camera.hpp"
#include "../../../SceneSystem/Scene/Scene.hpp"
#include "../../../Objects/Abstracts/APrimitive/APrimitive.hpp"
#include <vector>

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
            screenPoint.z - this->_position.z).normalize();
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
    Color Camera::castRay(Geometry::Ray& ray, Scene& scene)
    {
        std::optional<Geometry::HitRecord> closest;
        APrimitive* hitPrimitive = nullptr;

        // Find the closest intersection point along the ray
        for (const auto &primitive : scene.getPrimitives()) {
            std::optional<Geometry::HitRecord> hit = primitive->hit(ray);
            if (hit && (!closest || hit->rayDistance < closest->rayDistance)) {
                closest = hit;
                hitPrimitive = primitive.get(); // Safe to use get() since the raw pointer is used only within this scope and the unique_ptr owns the lifetime
            }
        }

        if (!closest)
            return Color(0, 0, 0);  // No intersection: return black

        // Compute shadow and base material color
        bool inShadow = computeShadow(closest.value(), scene);
        
        Color finalColor = hitPrimitive->getMaterial()->computeColor(closest.value(), scene.getLights());

        if (inShadow)
            finalColor = finalColor * 0.1f;  // Darken by 90% if in shadow

        return finalColor.clamp();
    }
    
    /**
     * @brief Determine if a hit point is in shadow.
     *
     * For each light in the scene:
     * 1. Compute the direction from the hit point toward the light
     * 2. Create a shadow ray from the hit point (with small epsilon offset to avoid self-intersection)
     * 3. Check if any primitive blocks the ray before reaching the light
     * 4. If any primitive blocks, the point is in shadow
     *
     * Shadow bias (0.001f): Small offset along surface normal to prevent self-shadowing artifacts
     *
     * @param hit The hit record at the intersection point
     * @param scene Reference to the scene containing primitives and lights
     * @return true if the hit point is in shadow from any light, false otherwise
     */
    bool Camera::computeShadow(Geometry::HitRecord &hit, Scene &scene)
    {
        for (const auto &light : scene.getLights()) {
            Geometry::Vector3D toLight = light->getPosition() - hit.point;
            float distanceToLight = toLight.length();
            Geometry::Vector3D direction = toLight.normalize();

            // Use small epsilon offset (shadow bias) to avoid self-intersection artifacts
            Geometry::Point3D shadowOrigin = hit.point + direction * 0.001f;
            Geometry::Ray shadowRay(shadowOrigin, direction);
            for (const auto &primitive : scene.getPrimitives()) {
                std::optional<Geometry::HitRecord> shadowHit = primitive->hit(shadowRay);
                if (shadowHit && shadowHit->rayDistance < distanceToLight)
                    return true;  // Primitive blocks light: in shadow
            }
        }
        return false;  // No primitives block light: not in shadow
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
    std::vector<std::vector<Color>> Camera::render(Scene& scene)
    {
        std::vector<std::vector<Color>> frameBuffer;
        frameBuffer.resize(_height, std::vector<Color>(_width));

        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                double u = static_cast<double>(x) / (_width - 1);
                double v = static_cast<double>(y) / (_height - 1);
                Geometry::Ray ray = generateRay(u, v);
                frameBuffer[y][x] = castRay(ray, scene);
            }
        }

        return frameBuffer;
    }

} // namespace RayTracer
