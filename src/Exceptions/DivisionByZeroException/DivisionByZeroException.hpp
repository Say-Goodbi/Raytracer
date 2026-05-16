#pragma once
#include "../RaytracerException/RaytracerException.hpp"

namespace RayTracer
{
    class DivisionByZeroException : public Exception
    {
        public:
            DivisionByZeroException(const std::source_location &loc = std::source_location::current())
                : Exception("Division by zero occured", loc) {}
    };
}