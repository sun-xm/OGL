#pragma once

#include "GLCommon.h"
#include <GL/glew.h>

class GLLight
{
public:
    GLLight(int index) : index(index)
    {
        this->Ambient(0.f, 0.f, 0.f, 0.f);
        this->Diffuse(0.f, 0.f, 0.f, 0.f);
        this->Specular(0.f, 0.f, 0.f, 0.f);
        this->Position(0.f, 0.f, 0.f, 0.f);
    }
    virtual ~GLLight()
    {
        this->Disable();
    }

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
        this->diffuse = { r, g, b, a };
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
    void Position(float x, float y, float z, float w)
    {
        this->position = { x, y, z, w };
    }
    void Position(const Vector<4>& position)
    {
        this->position = position;
    }
    void Position(const Vector<3>& position)
    {
        this->Position(position[0], position[1], position[2], 1.f);
    }

    void Enable()
    {
        glLightfv(GL_LIGHT0 + this->index, GL_AMBIENT,  this->ambient);
        glLightfv(GL_LIGHT0 + this->index, GL_DIFFUSE,  this->diffuse);
        glLightfv(GL_LIGHT0 + this->index, GL_SPECULAR, this->specular);
        glLightfv(GL_LIGHT0 + this->index, GL_POSITION, this->position);

        glEnable(GL_LIGHT0 + this->index);
    }
    void Disable()
    {
        glDisable(GL_LIGHT0 + this->index);
    }

protected:
    int index;

    Vector<4> ambient;
    Vector<4> diffuse;
    Vector<4> specular;
    Vector<4> position;
};