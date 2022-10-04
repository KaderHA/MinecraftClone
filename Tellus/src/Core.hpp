#pragma once
#include <stdio.h>
#include <memory>

#ifdef _WIN32
#    include <Windows.h>
#    define TS_ASSERT(x, y)                                                     \
        {                                                                       \
            if (!(x)) {                                                         \
                printf("Assertion Failed [%s][%d]: %s", __FILE__, __LINE__, y); \
                DebugBreak();                                                   \
            }                                                                   \
        }
#else
#    define TS_ASSERT(x, y)
#endif

namespace ts {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

};  // namespace ts