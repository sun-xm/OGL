#pragma once

#include "GLCommon.h"
#include <gl/glew.h>

class GLBuffer
{
public:
    GLBuffer(GLenum target = GL_ARRAY_BUFFER);
    GLBuffer(const GLBuffer& other);

    void Bind() const;
    bool Data(const void* data, GLsizeiptr size, GLenum usage);
    bool Copy(const GLBuffer& other, GLenum usage);
    GLint Size() const;
    GLenum Usage() const;

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

    GLBuffer& operator=(const GLBuffer&) = delete;

private:
    bool Create();

private:
    GLuint buffer;
    GLenum target;
};

class GLElmBuf : public GLBuffer
{
public:
    GLElmBuf() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER) {}
};