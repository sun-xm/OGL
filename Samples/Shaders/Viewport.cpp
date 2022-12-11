#include "Viewport.h"
#include "Cleanup.h"
#include <fstream>
#include <string>

using namespace std;

Viewport::Viewport()
{
}

bool Viewport::OnCreated()
{
    if (!GLWindow::OnCreated())
    {
        return false;
    }

    this->lightPos = { 0, 0, 1 };

    this->RegisterMessage(WM_LBUTTONDOWN, [this]
    {
        SetCapture(this->Handle());
        GetCursorPos(&this->pos);
        return 0;
    });

    this->RegisterMessage(WM_LBUTTONUP, []
    {
        ReleaseCapture();
        return 0;
    });

    this->RegisterMessage(WM_LBUTTONDBLCLK, [this]
    {
        this->triangle.Rotation = { 0, 0, 0, 0 };
        this->Invalidate();
        return 0;
    });

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        if (MK_LBUTTON & this->wparam)
        {
            POINT pos;
            GetCursorPos(&pos);

            auto x = pos.x - this->pos.x;
            auto y = pos.y - this->pos.y;
            this->pos = pos;

            auto qx = Quaternion<>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
            auto qy = Quaternion<>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
            auto qr = Quaternion<>::FromRotation(this->triangle.Rotation);

            this->triangle.Rotation = (qx * qy * qr).ToRotation();
            this->Invalidate();
        }
        return 0;
    });

    return true;
}

void Viewport::OnPaint()
{
    if (this->AttachContext())
    {
        glViewport(0, 0, this->ClientWidth(), this->ClientHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        this->program.Use();
        this->program.UniformFlt("Ambient", 0.2f);
        this->program.UniformV3f("LightPos", this->lightPos);
        this->program.UniformM4f("WorldView", Transform3D<>::LookAt({ 0, 0, 4 }, { 0, 0, 0 }, 0.f));
        this->program.UniformM4f("Projection", Transform3D<>::Perspective(ToRadian(45.f), (float)this->ClientWidth() / this->ClientHeight(), 0.01f, 100.f));

        this->sphere.Render(this->program, this->lightPos);
        this->triangle.Render(this->program, this->lightPos);

        this->SwapBuffers();
        this->DetachContext();
    }

    GLWindow::OnPaint();
}

bool Viewport::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    GLVShader vshader;
    GLFShader fshader;
    ONCLEANUP(vshader, [&]{ vshader.Release(); });
    ONCLEANUP(fshader, [&]{ fshader.Release(); });

    ifstream vsrc(L"vshader.glsl");
    ifstream fsrc(L"fshader.glsl");
    if (!vsrc.is_open() ||
        !fsrc.is_open())
    {
        return false;
    }

    if (!vshader.Source(vsrc) || !vshader.Compile())
    {
        OutputDebugStringA(("Failed to load vertex shader\n" + vshader.Log() + '\n').c_str());
        return false;
    }

    if (!fshader.Source(fsrc) || !fshader.Compile())
    {
        OutputDebugStringA(("Failed to load fragment shader\n" + fshader.Log() + '\n').c_str());
        return false;
    }

    this->program.Create();
    if (!this->program.Link(vshader, fshader))
    {
        OutputDebugStringA(("Failed to create link program\n" + this->program.Log() + '\n').c_str());
        return false;
    }

    this->sphere.Create();
    this->sphere.Position = { 1, 0, 0 };

    this->triangle.Create();
    this->triangle.Position = { -1, 0, 0 };

    return true;
}

void Viewport::OnContextDestroy()
{
    this->sphere.Release();
    this->triangle.Release();
    this->program.Release();
    GLWindow::OnContextDestroy();
}