#pragma once

#include "GLCommon.h"
#include "GLLight.h"
#include "GLCamera.h"
#include <gl/glew.h>

class GLScene
{
public:
    GLScene();
   ~GLScene();

    GLCamera& Camera();

    void DepthTest(bool enable);

    GLLight& EnableLight(int index);
    void DisableLight(int index);

    const Matrix<float, 4>& WorldView() const;

    virtual void Begin(int width, int height);
    virtual void End();

    int Width() const
    {
        return this->width;
    }
    int Height() const
    {
        return this->height;
    }

protected:
    bool enableDepth;

    GLCamera camera;
    GLLight* lights[GL_LIGHT7 - GL_LIGHT0];

    Matrix<float, 4> matrix;

    int width, height;
};