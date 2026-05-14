#pragma once
#include "../RaytracerException/RaytracerException.hpp"

namespace RayTracer
{
    class ParsingException : public Exception
    {
        public:
            ParsingException(const std::string &message, const std::source_location &loc = std::source_location::current())
                : Exception("Parsing :" + message, loc) {}
    };
}