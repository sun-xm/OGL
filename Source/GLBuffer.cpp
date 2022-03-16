#include "GLBuffer.h"

GLBuffer::GLBuffer(GLenum target) : buffer(0), target(target)
{
}

void GLBuffer::Release()
{
    if (this->buffer)
    {
        glDeleteBuffers(1, &this->buffer);
        this->buffer = 0;
    }
}

void GLBuffer::Bind() const
{
    glBindBuffer(this->target, this->buffer);
}

bool GLBuffer::Data(const void* data, GLsizeiptr size, GLenum usage)
{
    if (!data || !size)
    {
        this->Release();
        return true;
    }

    if (!this->Create())
    {
        return false;
    }

    this->Bind();
    glBufferData(this->target, size, data, usage);

    return true;
}

GLint GLBuffer::Size() const
{
    GLint size = 0;

    if (this->buffer)
    {
        this->Bind();
        glGetBufferParameteriv(this->target, GL_BUFFER_SIZE, &size);
    }

    return size;
}

bool GLBuffer::Create()
{
    if (!this->buffer)
    {
        glGenBuffers(1, &this->buffer);
    }
    return !!this->buffer;
}