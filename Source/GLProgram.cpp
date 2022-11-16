#include "GLProgram.h"

using namespace std;

GLProgram::GLProgram() : program(0)
{
}

bool GLProgram::Create()
{
    if (!this->program)
    {
        this->program = glCreateProgram();
    }

    return !!this->program;
}

void GLProgram::Release()
{
    glDeleteProgram(this->program);
    this->program = 0;
}

bool GLProgram::Use()
{
    if (!this->program)
    {
        return false;
    }

    glUseProgram(this->program);

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

void GLProgram::Attach(const GLShader& shader)
{
    glAttachShader(this->program, shader);
}

void GLProgram::Detach(const GLShader& shader)
{
    glDetachShader(this->program, shader);
}

bool GLProgram::Link(const GLVShader& vshader, const GLFShader& fshader)
{
    this->Attach(vshader);
    this->Attach(fshader);

    auto res = this->Link();

    this->Detach(vshader);
    this->Detach(fshader);

    return res;
}

bool GLProgram::Link()
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

bool GLProgram::BindAttrib(const string& name, const GLBuffer& buffer, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
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

void GLProgram::UnbindAttrib(const string& name)
{
    auto loc = this->GetAttribLocation(name);
    if (loc < 0)
    {
        return;
    }

    glDisableVertexAttribArray(loc);
}

bool GLProgram::BindAttribLocation(GLuint index, const string& name)
{
    glBindAttribLocation(this->program, index, name.c_str());

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

GLint GLProgram::GetAttribLocation(const string& name)
{
    return glGetAttribLocation(this->program, name.c_str());
}

bool GLProgram::BindFragDataLocation(GLuint index, const string& name)
{
    glBindFragDataLocation(this->program, index, name.c_str());

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

GLint GLProgram::GetFragDataLocation(const string& name)
{
    return glGetFragDataLocation(this->program, name.c_str());
}

bool GLProgram::Uniform1i(const string& name, int value)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniform1i(loc, value);
    return true;
}

bool GLProgram::UniformV2f(const string& name, const Vector<2>& value)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniform2f(loc, value[0], value[1]);
    return true;
}

bool GLProgram::UniformV3f(const string& name, const Vector<3>& value)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniform3f(loc, value[0], value[1], value[2]);
    return true;
}

bool GLProgram::UniformM3f(const string& name, const Matrix<3>& value, bool transpose)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniformMatrix3fv(loc, 1, transpose ? GL_TRUE : GL_FALSE, value);
    return true;
}

bool GLProgram::UniformV4f(const string& name, const Vector<4>& value)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniform4f(loc, value[0], value[1], value[2], value[3]);
    return true;
}

bool GLProgram::UniformM4f(const string& name, const Matrix<4>& value, bool transpose)
{
    auto loc = this->UniformLocation(name);
    if (loc < 0)
    {
        return false;
    }

    glUniformMatrix4fv(loc, 1, transpose ? GL_TRUE : GL_FALSE, value);
    return true;
}

bool GLProgram::UniformTex(const string& name, const GLTexture& texture, uint32_t index)
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

GLint GLProgram::UniformLocation(const string& name)
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