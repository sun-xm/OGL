#pragma once

#include "GLObject.h"
#include "GLProgram.h"
#include <memory>

class Triangle : public GLObject
{
public:
    Triangle();

    void Create();
    void Release();

    bool Colors(const Vector<3>* colors, int count);
    void Render(GLProgram& program, const Vertex& lightPos);

protected:
    GLBuffer  vbo, nbo, cbo;
};