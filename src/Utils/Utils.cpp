/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, May, 2026                                                     *
 * Title           - raytracer                                                          *
 * Description     -                                                                    *
 *     Utils                                                                            *
 * ------------------------------------------------------------------------------------ */

#include "Utils.hpp"

namespace RayTracer
{
    /**
     * @brief Generate a random double in the range [0, 1) using a thread-local random number generator.
     * Which permits thread safed random number generation without contention between threads.
     * @return A random double in the range [0, 1)
    **/
    double Utils::rng(void)
    {
        thread_local std::mt19937 gen{std::random_device{}()};
        thread_local std::uniform_real_distribution<double> dist{0.0, 1.0};
        return dist(gen);
    };

} // namespace RayTracer
