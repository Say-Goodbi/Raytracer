#include "RaytracerException.hpp"

const char *RayTracer::Exception::what() const noexcept
{
    static std::string result =  _message + " (at " + _location.file_name() + ":" + 
             std::to_string(_location.line()) + ":" + std::to_string(_location.column()) + ")";
    return result.c_str();
}
