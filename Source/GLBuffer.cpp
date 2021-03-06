#include "GLBuffer.h"

GLBuffer::GLBuffer(GLenum target) : buffer(obj), target(target)
{
    this->buffer = 0;

    this->create = [this]
    {
        glGenBuffers(1, &this->buffer);
        return !!this->buffer;
    };

    this->destroy = [this]
    {
        glDeleteBuffers(1, &this->buffer);
    };

    this->reset = [this]
    {
        this->buffer = 0;
    };
}

GLBuffer::GLBuffer(const GLBuffer& other) : GLBuffer(other.target)
{
    *this = other;
}

void GLBuffer::Bind() const
{
    glBindBuffer(this->target, this->buffer);
}

bool GLBuffer::Data(const void* data, GLsizeiptr size, GLenum usage)
{
    if (!this->buffer && !this->Create())
    {
        return false;
    }

    this->Bind();
    glBufferData(this->target, size, data, usage);

    return true;
}

bool GLBuffer::Copy(const GLBuffer& other, GLenum usage)
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

GLenum GLBuffer::Usage() const
{
    GLenum usage = 0;

    if (this->buffer)
    {
        this->Bind();
        glGetBufferParameteriv(this->target, GL_BUFFER_USAGE, (GLint*)&usage);
    }

    return usage;
}