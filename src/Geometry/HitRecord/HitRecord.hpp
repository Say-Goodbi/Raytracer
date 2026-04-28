/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Apr, 2026                                                     *
 * Title           - raytracer                                                          *
 * Description     -                                                                    *
 *     HitRecord                                                                        *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include "../Point3D/Point3D.hpp"
#include "../Vector3D/Vector3D.hpp"

namespace Geometry
{
    struct HitRecord
    {
        Point3D point;
        Vector3D normal;
        float rayDistance;
        // TO DO: add IMaterial here
    };
    
} // namespace Geometry