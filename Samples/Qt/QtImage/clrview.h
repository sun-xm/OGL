#pragma once

#include "glview.h"

class ClrView : public GLView
{
    Q_OBJECT

public:
    explicit ClrView(QWidget* parent);
    ~ClrView();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    QOpenGLShaderProgram* program;
    GLuint vbo;
    Matrix<3> transform;

    float w, h;
};