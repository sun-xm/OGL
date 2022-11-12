#pragma once

#include "glview.h"

class ImgView : public GLView
{
    Q_OBJECT

public:
    explicit ImgView(QWidget* parent);
    ~ImgView();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* program;
    GLuint vbo, tbo;
    GLuint tex;
};