#pragma once

#include "GLCommon.h"
#include <gl/glew.h>
#include <string>
#include <vector>

class GLShader
{
public:
    GLShader(GLenum type);

    virtual bool Create();
    virtual void Release();
    virtual bool Load(const std::wstring& path);

    void Source(const std::string& source);
    void Source(const std::vector<std::string>& sources);
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

protected:
    GLuint shader;
    GLenum type;
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