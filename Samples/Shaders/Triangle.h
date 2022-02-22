#pragma once

#include "GLShape.h"
#include "Program.h"

class Triangle : public GLShape
{
public:
    Triangle(Program& program);

    void Create();
    void Release() override;

    bool Colors(const Vector<float, 3>* colors, int count);
    void Render(const Vertex& lightPos, const Matrix<float, 4>& world);

protected:
    size_t ApplyVertices() override;
    void   RevokeVertices() override;
    void   ApplyProgram() override;

    Program& program;
    GLBuffer cbo;

    Vector<float, 3> lightPos;
    Matrix<float, 4> worldView;
};