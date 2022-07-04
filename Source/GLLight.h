#pragma once

#include "GLCommon.h"
#include <gl/glew.h>

class GLLight
{
public:
    GLLight(int index);
   ~GLLight();

    void Ambient(float r, float g, float b, float a);
    void Ambient(const Vector<4>& rgba);
    void Diffuse(float r, float g, float b, float a);
    void Diffuse(const Vector<4>& rgba);
    void Specular(float r, float g, float b, float a);
    void Specular(const Vector<4>& rgba);
    void Position(float x, float y, float z, float w);
    void Position(const Vector<4>& position);
    void Position(const Vector<3>& position);

    void Enable();
    void Disable();

private:
    int index;

    Vector<4> ambient;
    Vector<4> diffuse;
    Vector<4> specular;
    Vector<4> position;
};