/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2026                                                     *
 * Title           - raytracer                                                          *
 * Description     -                                                                    *
 *     rand                                                                             *
 * ------------------------------------------------------------------------------------ */

/**
 * @file src/Utils/Utils.hpp
 * @brief Utility declarations for common helpers used by the renderer.
 *
 * Declares small helper functions and constants used around the project. See
 * corresponding implementations in @ref src/Utils/Utils.cpp.
 */
 #pragma once
#include <random>

namespace RayTracer
{
    class Utils
    {
    public:
        static double rng(void);
    };
    
    
} // namespace RayTracer

