#pragma once

#include <GLProgram.h>
#include <QOpenGLWidget>

class ImgView : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ImgView(QWidget* parent);
    ~ImgView();

protected:
    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;

private:
    GLBuffer vbo, tbo;
    GLTexture tex;
    GLProgram program;

    Matrix<3> unify, origin;
};