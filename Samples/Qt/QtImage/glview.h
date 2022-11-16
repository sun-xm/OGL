#pragma once

#include "GLCommon.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLView(QWidget* parent);
    virtual ~GLView();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;

    void Coordinates(const std::vector<Vector<2>>& coords, GLuint buffer, GLenum usage);

    bool BindAttrib(GLuint program, const QString& name, GLuint buffer, GLint size, GLenum type, GLboolean normalized = GL_FALSE, GLsizei stride = 0 , const void* pointer = 0);
    void UnbindAttrib(GLuint program, const QString& name);

    bool UniformM3f(GLuint program, const QString& name, const Matrix<3>& value, GLboolean transpose = GL_TRUE);
    bool UniformV4f(GLuint program, const QString& name, const Vector<4>& value);
    bool UniformM4f(GLuint program, const QString& name, const Matrix<4>& value, GLboolean transpose = GL_TRUE);
    bool UniformTex(GLuint program, const QString& name, GLuint texture, GLenum target, uint32_t index);

    GLint BufferSize(GLuint buffer);

protected:
    Matrix<3> origin;
    Matrix<3> unify;
};