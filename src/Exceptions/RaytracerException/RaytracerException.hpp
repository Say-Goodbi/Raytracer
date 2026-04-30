#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <source_location>

namespace RayTracer
{
    class Exception : public std::exception
    {
    private:
        std::string _message;
        std::source_location _location;
    public:
        Exception(const std::string &message, const std::source_location &loc = std::source_location::current()) : _message(message), _location(loc) {}
        virtual const char *what() const noexcept override;
    };
}
