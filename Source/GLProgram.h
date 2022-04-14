#pragma once

#include "GLCommon.h"
#include "GLBuffer.h"
#include "GLShader.h"

class GLProgram
{
public:
    GLProgram();

    virtual bool Create();
    virtual void Release();
    virtual void Use();

    void Attach(const GLShader&);
    void Detach(const GLShader&);
    bool Link(std::string& log);
    bool Link();

    bool BindAttrib(const std::string& name, const GLBuffer& buffer, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const void* pointer = 0);
    void UnbindAttrib(const std::string& name);

    void  BindAttribLocation(GLuint index, const std::string& name);
    GLint GetAttribLocation(const std::string& name);
    void  BindFragDataLocation(GLuint index, const std::string& name);
    GLint GetFragDataLocation(const std::string& name);

    bool Uniform1i(const std::string& name, int value);
    bool UniformV2f(const std::string& name, const Vector<float, 2>& value);
    bool UniformV3f(const std::string& name, const Vector<float, 3>& value);
    bool UniformM3f(const std::string& name, const Matrix<float, 3>& value, bool transpose = true);
    bool UniformV4f(const std::string& name, const Vector<float, 4>& value);
    bool UniformM4f(const std::string& name, const Matrix<float, 4>& value, bool transpose = true);

    operator bool() const
    {
        return !!this->program;
    }
    operator GLuint() const
    {
        return this->program;
    }

private:
    GLint UniformLocation(const std::string& name);

protected:
    GLuint program;
};