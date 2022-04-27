#pragma once

#include "GLReference.h"
#include "GLCommon.h"
#include <gl/glew.h>
#include <iostream>
#include <string>
#include <vector>

class GLShader : public GLReference<GLuint>
{
public:
    GLShader(GLenum type);
    GLShader(const GLShader&);

    bool Source(std::istream& source);
    bool Source(const std::string& source);
    bool Source(const std::vector<std::string>& sources);
    bool Compile(std::string& log);
    bool Compile();

    operator bool() const
    {
        return !!this->shader;
    }
    operator GLuint() const
    {
        return this->shader;
    }

    GLShader& operator=(const GLShader& other)
    {
        this->type = other.type;
        return (GLShader&)GLReference<GLuint>::operator=(other);
    }

protected:
    GLenum  type;
    GLuint& shader;
};

class GLVShader : public GLShader
{
public:
    GLVShader() : GLShader(GL_VERTEX_SHADER) {}
};

class GLFShader : public GLShader
{
public:
    GLFShader() : GLShader(GL_FRAGMENT_SHADER) {}
};