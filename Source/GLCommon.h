#pragma once

#include "VectorX.h"
#include <cstdint>

typedef Vector<3, uint32_t> Element;
typedef Vector<3> Vertex;
typedef Vector<3> Normal;
typedef Vector<4> Color;
typedef Vector<2> Coordinate;

template<typename T, size_t N>
char(&_ArraySizeHelper(T(&array)[N]))[N];
#define COUNTOF(array)(sizeof( _ArraySizeHelper(array)))