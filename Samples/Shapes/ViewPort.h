#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "GLShape.h"

class ViewPort : public GLWindow
{
public:
    ViewPort();

    void CreateShape(int shape);

protected:
    bool OnContextCreated() override;
    void OnContextDestroy() override;
    void OnPaint() override;

private:
    void Render();

private:
    POINT    cursor;
    GLScene  scene;
    GLShape* shape;
};