#include "GLWindow.h"

GLWindow::GLWindow() : hdc(nullptr), hrc(nullptr)
{
}

GLWindow::~GLWindow()
{
}

bool GLWindow::AttachContext()
{
    return this->hrc && wglMakeCurrent(this->hdc, this->hrc);
}

void GLWindow::DetachContext()
{
    wglMakeCurrent(nullptr, nullptr);
}

void GLWindow::SwapBuffers()
{
    ::SwapBuffers(this->hdc);
}

bool GLWindow::OnCreated()
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

void GLWindow::OnDestroy()
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

bool GLWindow::OnContextCreated()
{
    return true;
}

void GLWindow::OnContextDestroy()
{
}