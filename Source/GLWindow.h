#pragma once

#include "Window.h"
#include <gl/glew.h>

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
    void OnClose() override;

    virtual bool OnContextCreated();
    virtual void OnContextDestroy();

protected:
    HDC   hdc;
    HGLRC hrc;
};