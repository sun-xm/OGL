#include "glview.h"

using namespace std;

GLView::GLView(QWidget* parent) : QOpenGLWidget(parent)
{
}

void GLView::initializeGL()
{
    initializeOpenGLFunctions();
}

void GLView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    this->origin = Transform2D<>::Shift(-w * .5f, h * .5f);
    this->unify  = Transform2D<>::Scale(2.f / w, 2.f / h);
}

void GLView::Coordinates(const vector<Vector<2>>& coords, GLuint buffer, GLenum usage)
{
    vector<Vertex> vertices;
    vertices.reserve(coords.size());

    for (auto& c : coords)
    {
        vertices.push_back(Vertex{ c.X, -c.Y, 1 });
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), usage);
}

bool GLView::BindAttrib(GLuint program, const QString& name, GLuint buffer, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
    auto loc = glGetAttribLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(loc);

    return true;
}

void GLView::UnbindAttrib(GLuint program, const QString& name)
{
    auto loc = glGetAttribLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return;
    }

    glDisableVertexAttribArray(loc);
}

bool GLView::UniformM3f(GLuint program, const QString& name, const Matrix<3>& value, GLboolean transpose)
{
    auto loc = glGetUniformLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniformMatrix3fv(loc, 1, transpose, value);

    return true;
}

bool GLView::UniformV4f(GLuint program, const QString& name, const Vector<4>& value)
{
    auto loc = glGetUniformLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniform4f(loc, value[0], value[1], value[2], value[3]);

    return true;
}

bool GLView::UniformM4f(GLuint program, const QString& name, const Matrix<4>& value, GLboolean transpose)
{
    auto loc = glGetUniformLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return false;
    }

    glUniformMatrix4fv(loc, 1, transpose, value);

    return true;
}

bool GLView::UniformTex(GLuint program, const QString& name, GLuint texture, GLenum target, uint32_t index)
{
    auto loc = glGetUniformLocation(program, name.toStdString().c_str());
    if (loc < 0)
    {
        return false;
    }

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(target, texture);
    glUniform1i(loc, index);

    return true;
}

GLint GLView::BufferSize(GLuint buffer)
{
    GLint size = 0;
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    return size;
}