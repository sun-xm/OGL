#pragma once

#include "GLShape.h"
#include "GLProgram.h"

class Triangle : public GLShape
{
public:
    Triangle();

    void Create(const GLProgram& program);
    void Release() override;

    bool Colors(const Vector<float, 3>* colors, int count);
    void Render(const GLScene& scene, const Vertex& lightPos);

protected:
    size_t Apply(const GLScene& scene) override;
    void   Revoke() override;

    GLProgram program;
    GLBuffer  cbo;

    Vector<float, 3> lightPos;
};