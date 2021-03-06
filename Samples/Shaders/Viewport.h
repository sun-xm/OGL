#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "GLProgram.h"
#include "Sphere.h"
#include "Triangle.h"

class Viewport : public GLWindow
{
public:
    Viewport();

protected:
    bool OnCreated() override;
    void OnPaint() override;

    bool OnContextCreated() override;
    void OnContextDestroy() override;

private:
    GLScene   scene;
    GLProgram program;
    Sphere    sphere;
    Triangle  triangle;

    Vertex lightPos;

    POINT pos;
};