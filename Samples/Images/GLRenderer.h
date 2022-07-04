#pragma once

#include "GLTexture.h"
#include "GLProgram.h"
#include "GLShader.h"
#include "GLCommon.h"
#include <string>

class GLRenderer
{
public:
    GLRenderer();

    bool Create(std::string& log);
    void Release();

    void Identity();
    void Rotate(float x, float y, float radian);
    void Scale(float x, float y);
    void Shift(float x, float y);

    Matrix<3> Transform() const
    {
        return this->transform;
    }
    void Transform(const Matrix<3>& transform)
    {
        this->transform = transform;
    }

    void Begin(int w, int h);
    void End();

    void Clear(float r, float g, float b, float a = 1.f);
    void Triangles(const Coordinate* screenCoords, size_t count, const Vector<4>& color);
    void Triangles(const Coordinate* screenCoords, const Coordinate* texCoords, size_t count, const GLTexture& texture);
    void Image(const GLTexture& texture, float w, float h);
    void Line(float x0, float y0, float x1, float y1);

private:
    GLProgram texProg;
    GLProgram clrProg;

    Matrix<3> transform;
    Matrix<3> origin;
    Matrix<3> unify;
};