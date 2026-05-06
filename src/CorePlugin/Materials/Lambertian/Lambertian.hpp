/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2026                                                     *
 * Title           - raytracer                                                          *
 * Description     -                                                                    *
 *     Lambertian                                                                       *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                     *
 *             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                     *
 *             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                     *
 *             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                     *
 *             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                     *
 *             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                     *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include "../../../Utils/Color.hpp"
#include "../../../Objects/Abstracts/IMaterial.hpp"
namespace RayTracer
{
    class Lambertian : public IMaterial
    {
        private:
            Color _albedo;
        protected:
        public:
            Lambertian(const Color& albedo);
            ~Lambertian() = default;

            Color evaluate(const Geometry::Vector3D&, const Geometry::Vector3D&, const Geometry::Vector3D&) const override;
            Geometry::Vector3D sample(const Geometry::Vector3D& normal, const Geometry::Vector3D&) const override;
            float pdf(const Geometry::Vector3D& normal, const Geometry::Vector3D&, const Geometry::Vector3D& sampleDir) const override;
    };
} // namespace RayTracer
