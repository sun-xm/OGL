#pragma once

#include "VectorX.h"
#include <cstdint>

#ifdef VECTORX_NS
    #define vx VECTORX_NS
#else
    #define vx
#endif

typedef vx::Vector<3, uint32_t> Element;
typedef vx::Vector<3, float>    Vertex;
typedef vx::Vector<3, float>    Normal;
typedef vx::Vector<4, float>    Color;
typedef vx::Vector<2, float>    Coordinate;

template<typename T, size_t N>
char(&_ArraySizeHelper(T(&)[N]))[N];
#define COUNTOF(arr)(sizeof( _ArraySizeHelper(arr)))