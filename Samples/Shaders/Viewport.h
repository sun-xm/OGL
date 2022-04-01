#pragma once

#include "GLWindow.h"
#include "GLScene.h"
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
    GLScene  scene;
    Program  program;
    Sphere   sphere;
    Triangle triangle;

    Vertex lightPos;

    POINT pos;
};