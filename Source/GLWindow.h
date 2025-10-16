#pragma once

#include <Window.h>
#include <GL/glew.h>

class GLWindow : public Window
{
public:
    GLWindow() : hdc(nullptr), hrc(nullptr) {}
    virtual ~GLWindow() {}

protected:
    bool AttachContext() { return this->hrc && wglMakeCurrent(this->hdc, this->hrc); }
    void DetachContext() { wglMakeCurrent(nullptr, nullptr); }
    void SwapBuffers() { ::SwapBuffers(this->hdc); }

    bool OnCreated() override
    {
        this->hdc = GetDC(this->hwnd);
        if (!this->hdc)
        {
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;

        auto pf = ChoosePixelFormat(this->hdc, &pfd);
        if (!pf)
        {
            return false;
        }

        if (!SetPixelFormat(this->hdc, pf, &pfd))
        {
            return false;
        }

        DescribePixelFormat(this->hdc, pf, sizeof(pfd), &pfd);

        this->hrc = wglCreateContext(this->hdc);
        if (!this->AttachContext())
        {
            return false;
        }

        if (GLEW_OK != glewInit())
        {
            return false;
        }

        if (!this->OnContextCreated())
        {
            this->DetachContext();
            return false;
        }

        this->DetachContext();
        return true;
    }
    void OnDestroy() override
    {
        if (this->AttachContext())
        {
            this->OnContextDestroy();
            this->DetachContext();
        }

        if (this->hdc)
        {
            if (this->hrc)
            {
                wglDeleteContext(this->hrc);
                this->hrc = nullptr;
            }

            ReleaseDC(this->hwnd, this->hdc);
            this->hdc = nullptr;
        }

        Window::OnDestroy();
    }

    virtual bool OnContextCreated() { return true; }
    virtual void OnContextDestroy() {}

protected:
    HDC   hdc;
    HGLRC hrc;
};