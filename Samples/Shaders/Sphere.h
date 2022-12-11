#pragma once

#include "GLObject.h"
#include "GLProgram.h"
#include <memory>
#include <vector>

class Sphere : public GLObject
{
public:
    Sphere();

    void Create();
    void Release();

    bool Colors(const Vector<3>* colors, int count);
    void Render(GLProgram& program, const Vertex& lightPos);

private:
    static void Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices);

    GLBuffer  vbo, nbo, cbo;
    Vector<3> lightPos;
};