#pragma once

#include "GLReference.h"
#include "GLCommon.h"
#include <gl/glew.h>

class GLBuffer : public GLReference<GLuint>
{
public:
    GLBuffer(GLenum target = GL_ARRAY_BUFFER);
    GLBuffer(const GLBuffer& other);

    void Bind() const;
    bool Data(const void* data, GLsizeiptr size, GLenum usage);
    bool Copy(const GLBuffer& other, GLenum usage = 0);
    GLint Size() const;
    GLenum Usage() const;

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

    GLBuffer& operator=(const GLBuffer& other)
    {
        this->target = other.target;
        return (GLBuffer&)GLReference<GLuint>::operator=(other);
    }

private:
    GLuint& buffer;
    GLenum  target;
};

class GLElmBuf : public GLBuffer
{
public:
    GLElmBuf() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER) {}
};