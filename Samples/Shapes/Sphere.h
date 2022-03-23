#pragma once

#include "GLShape.h"
#include <vector>

class Sphere : public GLShape
{
public:
    Sphere();

private:
    void Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices, std::vector<Coordinate>& coords);
};