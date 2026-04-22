#pragma once

#include "GLCommon.h"
#include "GLBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLVertexArray.h"

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

    template<typename... Shaders>
    bool Link(const GLShader& shader, const Shaders&... shaders)
    {
        if (!this->Attach(shader))
        {
            return false;
        }

        bool success = sizeof...(shaders) ? this->Link(shaders...) :
                                            this->Link();

        this->Detach(shader);
        return success;
    }

    bool BindAttrib(const std::string& name, const GLBuffer& buffer, size_t count, GLenum type, size_t stride = 0, size_t offset = 0, GLboolean normalized = GL_FALSE)
    {
        if (!buffer)
        {
            return false;
        }

        auto loc = this->GetAttribLocation(name);
        if (loc < 0)
        {
            return false;
        }

        buffer.Bind();

        glVertexAttribPointer(loc, (GLint)count, type, normalized, (GLsizei)stride, (const void*)offset);
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

    bool BindAttribLocation(GLuint index, const std::string& name)
    {
        glBindAttribLocation(this->program, index, name.c_str());

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool BindBuffer(const std::string& name, const GLBuffer& buffer)
    {
        if (GL_SHADER_STORAGE_BUFFER != buffer.Target())
        {
            return false;
        }

        auto binding = GetBufferBinding(name);
        if (binding < 0)
        {
            return false;
        }

        glBindBufferBase(buffer.Target(), binding, buffer);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool BindUniformAtomic(const std::string& name, const GLBuffer& buffer)
    {
        if (GL_ATOMIC_COUNTER_BUFFER != buffer.Target())
        {
            return false;
        }

        auto binding = GetUniformAtomicBinding(name);
        if (binding < 0)
        {
            return false;
        }

        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, binding, buffer);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool BindUniformBlock(const std::string& name, const GLBuffer& buffer)
    {
        if (GL_UNIFORM_BUFFER != buffer.Target())
        {
            return false;
        }

        auto binding = GetUniformBlockBinding(name);
        if (binding < 0)
        {
            return false;
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);

        auto err = glGetError();
        return GL_NO_ERROR == err;
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
        auto loc = this->GetUniformLocation(name);
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
        auto loc = this->GetUniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform1f(loc, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool UniformV2f(const std::string& name, const vx::Vector<2>& value)
    {
        auto loc = this->GetUniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform2f(loc, value[0], value[1]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool UniformV3f(const std::string& name, const vx::Vector<3>& value)
    {
        auto loc = this->GetUniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform3f(loc, value[0], value[1], value[2]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool UniformM3f(const std::string& name, const vx::Matrix<3>& value, bool transpose = true)
    {
        auto loc = this->GetUniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniformMatrix3fv(loc, 1, transpose ? GL_TRUE : GL_FALSE, value);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool UniformV4f(const std::string& name, const vx::Vector<4>& value)
    {
        auto loc = this->GetUniformLocation(name);
        if (loc < 0)
        {
            return false;
        }

        glUniform4f(loc, value[0], value[1], value[2], value[3]);

        auto err = glGetError();
        return GL_NO_ERROR == err;
    }

    bool UniformM4f(const std::string& name, const vx::Matrix<4>& value, bool transpose = true)
    {
        auto loc = this->GetUniformLocation(name);
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
        auto loc = this->GetUniformLocation(name);
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

    GLint GetAttribLocation(const std::string& name)
    {
        return glGetAttribLocation(this->program, name.c_str());
    }

    GLint GetBufferBinding(const std::string& name)
    {
        static const GLenum prop = GL_BUFFER_BINDING;

        auto block = glGetProgramResourceIndex(this->program, GL_SHADER_STORAGE_BLOCK, name.c_str());
        if (GL_INVALID_INDEX == block)
        {
            return -1;
        }

        GLint binding = -1;
        glGetProgramResourceiv(this->program, GL_SHADER_STORAGE_BLOCK, block, 1, &prop, 1, nullptr, &binding);
        return binding;
    }

    GLint GetUniformAtomicBinding(const std::string& name)
    {
        static const GLenum prop = GL_ATOMIC_COUNTER_BUFFER_INDEX;

        auto index = glGetProgramResourceIndex(this->program, GL_UNIFORM, name.c_str());
        if (GL_INVALID_INDEX == index)
        {
            return -1;
        }

        GLint atomic;
        glGetProgramResourceiv(this->program, GL_UNIFORM, index, 1, &prop, 1, nullptr, &atomic);

        GLint binding = -1;
        glGetActiveAtomicCounterBufferiv(this->program, atomic, GL_ATOMIC_COUNTER_BUFFER_BINDING, &binding);
        return binding;
    }

    GLint GetUniformBlockBinding(const std::string& name)
    {
        auto index = glGetUniformBlockIndex(this->program, name.c_str());
        if (GL_INVALID_INDEX == index)
        {
            return -1;
        }

        GLint binding = -1;
        glGetActiveUniformBlockiv(this->program, index, GL_UNIFORM_BLOCK_BINDING, &binding);
        return binding;
    }

    GLint GetUniformLocation(const std::string& name)
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