#pragma once

#include "GLCommon.h"
#include "GLBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"

class GLProgram
{
public:
    GLProgram() : program(0) {}
    GLProgram(const GLProgram&) = delete;

    bool Create()
    {
        if (!this->program)
        {
            this->program = glCreateProgram();
        }

        return !!this->program;
    }
    void Release()
    {
        glDeleteProgram(this->program);
        this->program = 0;
    }

    bool Use()
    {
        if (!this->program)
        {
            return false;
        }

        glUseProgram(this->program);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool Attach(const GLShader& shader)
    {
        if (!this->program && !this->Create())
        {
            return false;
        }

        glAttachShader(this->program, shader);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    void Detach(const GLShader& shader)
    {
        if (this->program)
        {
            glDetachShader(this->program, shader);
        }
    }

    bool Link(const GLVShader& vshader, const GLFShader& fshader)
    {
        if (!this->Attach(vshader))
        {
            return false;
        }

        if (!this->Attach(fshader))
        {
            this->Detach(fshader);
            return false;
        }

        auto res = this->Link();

        this->Detach(vshader);
        this->Detach(fshader);

        return res;
    }
    bool Link()
    {
        if (!this->program)
        {
            return false;
        }

        glLinkProgram(this->program);

        GLint status;
        glGetProgramiv(this->program, GL_LINK_STATUS, &status);

        if (!status)
        {
            GLchar  buf[4096];
            GLsizei len;
            glGetProgramInfoLog(this->program, 4096, &len, buf);
            this->log = buf;
            return false;
        }

        this->log.clear();
        return true;
    }

    bool BindAttrib(const std::string& name, const GLBuffer& buffer, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const void* pointer = 0)
    {
        auto loc = this->GetAttribLocation(name);
        if (loc < 0)
        {
            return false;
        }

        if (!buffer)
        {
            return false;
        }
        buffer.Bind();

        glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(loc);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    void UnbindAttrib(const std::string& name)
    {
        auto loc = this->GetAttribLocation(name);
        if (loc < 0)
        {
            return;
        }

        glDisableVertexAttribArray(loc);
    }

    bool  BindAttribLocation(GLuint index, const std::string& name)
    {
        glBindAttribLocation(this->program, index, name.c_str());

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    GLint GetAttribLocation(const std::string& name)
    {
        return glGetAttribLocation(this->program, name.c_str());
    }
    bool  BindFragDataLocation(GLuint index, const std::string& name)
    {
        glBindFragDataLocation(this->program, index, name.c_str());

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    GLint GetFragDataLocation(const std::string& name)
    {
        return glGetFragDataLocation(this->program, name.c_str());
    }

    bool UniformInt(const std::string& name, int value)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform1i(loc, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformFlt(const std::string& name, float value)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform1f(loc, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformV2f(const std::string& name, const Vector<2>& value)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform2f(loc, value[0], value[1]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformV3f(const std::string& name, const Vector<3>& value)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform3f(loc, value[0], value[1], value[2]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformM3f(const std::string& name, const Matrix<3>& value, bool transpose = true)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniformMatrix3fv(loc, 1, transpose ? GL_TRUE : GL_FALSE, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformV4f(const std::string& name, const Vector<4>& value)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform4f(loc, value[0], value[1], value[2], value[3]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformM4f(const std::string& name, const Matrix<4>& value, bool transpose = true)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniformMatrix4fv(loc, 1, transpose ? GL_TRUE : GL_FALSE, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }
    bool UniformTex(const std::string& name, const GLTexture& texture, uint32_t index)
    {
        auto loc = this->UniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(texture.Target(), texture);
        glUniform1i(loc, index);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    const std::string& Log()
    {
        return this->log;
    }

    operator bool() const
    {
        return !!this->program;
    }
    operator GLuint() const
    {
        return this->program;
    }

    GLProgram& operator=(const GLProgram& other) = delete;

private:
    GLint UniformLocation(const std::string& name)
    {
        if (!this->program)
        {
            return -1;
        }

        GLint program;
        glGetIntegerv(GL_CURRENT_PROGRAM, &program);
        if (program != this->program)
        {
            return -1;
        }

        return glGetUniformLocation(this->program, name.c_str());
    }

protected:
    GLuint program;
    std::string log;
};