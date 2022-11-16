#pragma once

#include "glview.h"
#include <QOpenGLShaderProgram>

class ImgView : public GLView
{
    Q_OBJECT

public:
    explicit ImgView(QWidget* parent);
    ~ImgView();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* program;
    GLuint vbo, tbo;
    GLuint tex;
};