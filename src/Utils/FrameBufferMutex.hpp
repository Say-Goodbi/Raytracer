#pragma once
#include <shared_mutex>

namespace RayTracer
{
    inline std::shared_mutex& FrameBufferMutex()
    {
        static std::shared_mutex mutex;
        return mutex;
    }
} // namespace RayTracer