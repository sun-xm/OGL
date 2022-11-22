#pragma once

#include <GL/glew.h>

class GLBuffer
{
public:
    GLBuffer(GLenum target = GL_ARRAY_BUFFER) : buffer(0), target(target) {}
    GLBuffer(const GLBuffer& other) = delete;
    virtual ~GLBuffer() {}

    bool Create()
    {
        if (!this->buffer)
        {
            glGenBuffers(1, &this->buffer);
        }

        return !!this->buffer;
    }
    void Release()
    {
        glDeleteBuffers(1, &this->buffer);
        this->buffer = 0;
    }

    void Bind() const
    {
        glBindBuffer(this->target, this->buffer);
    }
    GLint Size() const
    {
        GLint size = 0;

        if (this->buffer)
        {
            this->Bind();
            glGetBufferParameteriv(this->target, GL_BUFFER_SIZE, &size);
        }

        return size;
    }
    GLenum Usage() const
    {
        GLenum usage = 0;

        if (this->buffer)
        {
            this->Bind();
            glGetBufferParameteriv(this->target, GL_BUFFER_USAGE, (GLint*)&usage);
        }

        return usage;
    }
    GLenum Target() const
    {
        return this->target;
    }

    bool Data(const void* data, GLsizeiptr size, GLenum usage)
    {
        if (!this->buffer && !this->Create())
        {
            return false;
        }

        this->Bind();
        glBufferData(this->target, size, data, usage);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool Copy(const GLBuffer& other, GLenum usage = 0)
    {
        if (this->target != other.target)
        {
            return false;
        }

        if (!other)
        {
            return false;
        }

        if (!this->buffer && !this->Create())
        {
            return false;
        }

        auto size = other.Size();

        if (!usage)
        {
            usage = other.Usage();
        }

        this->Data(nullptr, size, usage);

        glBindBuffer(GL_COPY_READ_BUFFER,  other.buffer);
        glBindBuffer(GL_COPY_WRITE_BUFFER, this->buffer);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    operator bool() const
    {
        return !!this->buffer;
    }

    operator GLuint() const
    {
        return this->buffer;
    }

    GLBuffer& operator=(const GLBuffer& other) = delete;

protected:
    GLuint buffer;
    GLenum target;
};

class GLElmBuf : public GLBuffer
{
public:
    GLElmBuf() : GLBuffer(GL_ELEMENT_ARRAY_BUFFER) {}
};