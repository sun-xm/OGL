#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "GLProgram.h"
#include "Sphere.h"
#include "Triangle.h"
#include <memory>

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
    Sphere    sphere;
    Triangle  triangle;

    std::shared_ptr<GLProgram> program;

    Vertex lightPos;

    POINT pos;
};