#pragma once

#include "GLReference.h"
#include "GLCommon.h"
#include "GLBuffer.h"
#include "GLShader.h"
#include "GLTexture.h"

class GLProgram : public GLReference<GLuint>
{
public:
    GLProgram();
    GLProgram(const GLProgram&);

    void Use();

    void Attach(const GLShader&);
    void Detach(const GLShader&);
    bool Link(const GLVShader&, const GLFShader&, std::string& log);
    bool Link(std::string& log);
    bool Link();

    bool BindAttrib(const std::string& name, const GLBuffer& buffer, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0, const void* pointer = 0);
    void UnbindAttrib(const std::string& name);

    void  BindAttribLocation(GLuint index, const std::string& name);
    GLint GetAttribLocation(const std::string& name);
    void  BindFragDataLocation(GLuint index, const std::string& name);
    GLint GetFragDataLocation(const std::string& name);

    bool Uniform1i(const std::string& name, int value);
    bool UniformV2f(const std::string& name, const Vector<2>& value);
    bool UniformV3f(const std::string& name, const Vector<3>& value);
    bool UniformM3f(const std::string& name, const Matrix<3>& value, bool transpose = true);
    bool UniformV4f(const std::string& name, const Vector<4>& value);
    bool UniformM4f(const std::string& name, const Matrix<4>& value, bool transpose = true);
    bool UniformTex(const std::string& name, const GLTexture& texture, uint32_t index);

    operator bool() const
    {
        return !!this->program;
    }
    operator GLuint() const
    {
        return this->program;
    }

    GLProgram& operator=(const GLProgram& other)
    {
        return (GLProgram&)GLReference<GLuint>::operator=(other);
    }

private:
    GLint UniformLocation(const std::string& name);

protected:
    GLuint& program;
};