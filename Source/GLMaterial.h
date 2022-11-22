#pragma once

#include "GLCommon.h"
#include "GL/glew.h"

class GLMaterial
{
public:
    GLMaterial()
    {
        this->Ambient({ .2f, .2f, .2f, 1.f });
        this->Diffuse({ .8f, .8f, .8f, 1.f });
        this->Specular({ .0f, .0f, .0f, 1.f });
        this->Emission({ .0f, .0f, .0f, 1.f });
        this->Shininess(0.f);
    }
    virtual ~GLMaterial() {}

    void Ambient(float r, float g, float b, float a)
    {
        this->ambient = { r, g, b, a };
    }
    void Ambient(const Vector<4>& rgba)
    {
        this->ambient = rgba;
    }
    void Diffuse(float r, float g, float b, float a)
    {
        this->diffuse = { r, g, b, a};
    }
    void Diffuse(const Vector<4>& rgba)
    {
        this->diffuse = rgba;
    }
    void Specular(float r, float g, float b, float a)
    {
        this->specular = { r, g, b, a };
    }
    void Specular(const Vector<4>& rgba)
    {
        this->specular = rgba;
    }
    void Emission(float r, float g, float b, float a)
    {
        this->emission = { r, g, b, a };
    }
    void Emission(const Vector<4>& rgba)
    {
        this->emission = rgba;
    }
    void Shininess(float shininess)
    {
        this->shininess = shininess;
    }

    void Apply()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, this->emission);
        glMaterialf(GL_FRONT, GL_SHININESS, this->shininess);
    }
    void Revoke()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,  Vector<4>{ .2f, .2f, .2f, 1.f });
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  Vector<4>{ .8f, .8f, .8f, 1.f });
        glMaterialfv(GL_FRONT, GL_SPECULAR, Vector<4>{ .0f, .0f, .0f, 1.f });
        glMaterialfv(GL_FRONT, GL_EMISSION, Vector<4>{ .0f, .0f, .0f, 1.f });
        glMaterialf(GL_FRONT, GL_SHININESS, 0.f);
    }

protected:
    Vector<4> ambient;
    Vector<4> diffuse;
    Vector<4> specular;
    Vector<4> emission;

    float shininess;
};