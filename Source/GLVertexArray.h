#pragma once

#include <GL/glew.h>

class GLVertexArray
{
public:
    GLVertexArray() : array(0) {}

    bool Create()
    {
        if (!this->array)
        {
            glGenVertexArrays(1, &this->array);
        }

        return !!this->array;
    }

    void Release()
    {
        glDeleteVertexArrays(1, &this->array);
        this->array = 0;
    }

    void Bind() const
    {
        glBindVertexArray(this->array);
    }

    operator bool() const
    {
        return !!this->array;
    }

    operator GLuint() const
    {
        return this->array;
    }

private:
    GLuint array;
};