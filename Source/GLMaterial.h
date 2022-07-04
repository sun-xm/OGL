#pragma once

#include "GLCommon.h"

class GLMaterial
{
public:
    GLMaterial();

    void Ambient(float r, float g, float b, float a);
    void Ambient(const Vector<4>& rgba);
    void Diffuse(float r, float g, float b, float a);
    void Diffuse(const Vector<4>& rgba);
    void Specular(float r, float g, float b, float a);
    void Specular(const Vector<4>& rgba);
    void Emission(float r, float g, float b, float a);
    void Emission(const Vector<4>& rgba);
    void Shininess(float shininess);

    void Apply();
    void Revoke();

private:
    Vector<4> ambient;
    Vector<4> diffuse;
    Vector<4> specular;
    Vector<4> emission;

    float shininess;
};