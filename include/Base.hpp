#ifndef BASE_H_
#define BASE_H_

#include<_mingw.h>

#define LOG(...)
#define ASSERT(x, ...)        \
    {                         \
        if (!(x))             \
        {                     \
            LOG(__VA_ARGS__); \
            __debugbreak();   \
        }                     \
    }

#endif