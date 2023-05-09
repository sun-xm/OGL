#pragma once

#include "Window.h"
#include <GL/glew.h>

class GLWindow : public Window
{
public:
    GLWindow(HINSTANCE instance = nullptr);
    virtual ~GLWindow();

protected:
    bool AttachContext();
    void DetachContext();
    void SwapBuffers();

    bool OnCreated() override;
    void OnDestroy() override;

    virtual bool OnContextCreated();
    virtual void OnContextDestroy();

protected:
    HDC   hdc;
    HGLRC hrc;
};