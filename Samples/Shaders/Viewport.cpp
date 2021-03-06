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

    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().Position(0.f, 0.f, 4.f);

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
        this->scene.Begin(this->ClientWidth(), this->ClientHeight());

        this->sphere.Render(this->scene, this->lightPos);
        this->triangle.Render(this->scene, this->lightPos);

        this->scene.End();

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

    string log;
    if (!vshader.Source(vsrc) || !vshader.Compile(log) ||
        !fshader.Source(fsrc) || !fshader.Compile(log))
    {
        OutputDebugStringA(("Failed to create load shaders\n" + log + '\n').c_str());
        return false;
    }

    this->program.Create();
    if (!this->program.Link(vshader, fshader, log))
    {
        OutputDebugStringA(("Failed to create link program\n" + log + '\n').c_str());
        return false;
    }

    this->sphere.Create(this->program);
    this->sphere.Position = { 1, 0, 0 };

    this->triangle.Create(this->program);
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