#pragma once

#include "GLCommon.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>

class GLShader
{
public:
    GLShader(GLenum type) : shader(0), type(type) {}
    GLShader(const GLShader&) = delete;
    virtual ~GLShader() {}

    bool Create()
    {
        if (!this->shader)
        {
            this->shader = glCreateShader(this->type);
        }

        return !!this->shader;
    }
    void Release()
    {
        glDeleteShader(this->shader);
        this->shader = 0;
    }

    bool Source(std::istream& source)
    {
        return this->Source(std::string(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>()));
    }
    bool Source(const std::string& source)
    {
        if (!this->shader && !this->Create())
        {
            return false;
        }

        const GLchar* srcs[] = { source.c_str() };
        const GLint   lens[] = { (GLint)source.length() };
        glShaderSource(this->shader, 1, srcs, lens);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool Source(const std::vector<std::string>& sources)
    {
        if (!this->shader && !this->Create())
        {
            return false;
        }

        std::vector<const GLchar*> srcs(sources.size());
        std::vector<GLint> lens(sources.size());

        for (size_t i = 0; i < sources.size(); i++)
        {
            srcs[i] = sources[i].c_str();
            lens[i] = (GLint)sources[i].length();
        }

        glShaderSource(this->shader, (GLsizei)sources.size(), srcs.data(), lens.data());

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool Compile()
    {
        glCompileShader(this->shader);

        GLint status;
        glGetShaderiv(this->shader, GL_COMPILE_STATUS, &status);

        if (!status)
        {
            GLchar  buf[4096];
            GLsizei len;
            glGetShaderInfoLog(this->shader, 4096, &len, buf);
            this->log = buf;
            return false;
        }

        this->log.clear();
        return true;
    }

    const std::string& Log()
    {
        return this->log;
    }

    operator bool() const
    {
        return !!this->shader;
    }
    operator GLuint() const
    {
        return this->shader;
    }

    GLShader& operator=(const GLShader& other) = delete;

protected:
    GLenum type;
    GLuint shader;
    std::string log;
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