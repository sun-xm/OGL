#include "GLShader.h"
#include <fstream>

using namespace std;

GLShader::GLShader(GLenum type) : shader(0), type(type)
{
}

bool GLShader::Create()
{
    if (!this->shader)
    {
        this->shader = glCreateShader(this->type);
    }

    return !!this->shader;
}

void GLShader::Release()
{
    glDeleteShader(this->shader);
    this->shader = 0;
}

bool GLShader::Source(istream& source)
{
    return this->Source(string(istreambuf_iterator<char>(source), istreambuf_iterator<char>()));
}

bool GLShader::Source(const string& source)
{
    if (!this->shader && !this->Create())
    {
        return false;
    }
    const GLchar* srcs[] = { source.c_str() };
    const GLint   lens[] = { (GLint)source.length() };
    glShaderSource(this->shader, 1, srcs, lens);

    return true;
}

bool GLShader::Source(const vector<string>& sources)
{
    if (!this->shader && !this->Create())
    {
        return false;
    }

    vector<const GLchar*> srcs(sources.size());
    vector<GLint> lens(sources.size());

    for (size_t i = 0; i < sources.size(); i++)
    {
        srcs[i] = sources[i].c_str();
        lens[i] = (GLint)sources[i].length();
    }

    glShaderSource(this->shader, (GLsizei)sources.size(), srcs.data(), lens.data());

    return true;
}

bool GLShader::Compile()
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