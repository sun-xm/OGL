#pragma once

#include "GLCommon.h"
#include <gl/glew.h>

class GLBuffer
{
public:
    GLBuffer(GLenum target);

    void Bind() const;
    bool Data(const void* data, GLsizeiptr size, GLenum usage);
    GLint Size() const;

    void Release();

    GLenum Target() const
    {
        return this->target;
    }

    operator bool() const
    {
        return !!this->buffer;
    }

    operator GLuint() const
    {
        return this->buffer;
    }

private:
    bool Create();

private:
    GLuint buffer;
    GLenum target;
};