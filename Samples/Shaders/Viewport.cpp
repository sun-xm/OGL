#include "Viewport.h"
#include <string>

using namespace std;

Viewport::Viewport() : sphere(this->program), triangle(this->program)
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

            auto qx = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
            auto qy = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
            auto qr = Quaternion<float>::FromRotation(this->triangle.Rotation);

            this->triangle.Rotation = (qx * qy * qr).ToRotation();
            this->Invalidate();
        }
        return 0;
    });

    return true;
}

void Viewport::OnDestroy()
{
    this->sphere.Release();
    this->triangle.Release();
    this->program.Release();
    GLWindow::OnDestroy();
}

void Viewport::OnPaint()
{
    if (this->AttachContext())
    {
        this->scene.Begin(this->ClientWidth(), this->ClientHeight());

        Matrix<float, 4> worldView;
        glGetFloatv(GL_MODELVIEW_MATRIX, worldView);

        this->sphere.Render(this->lightPos, worldView);
        this->triangle.Render(this->lightPos, worldView);

        this->scene.End();
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

    string log;
    if (!this->program.Create(L"VertexShader.glsl", L"FragmentShader.glsl", log) || !this->program.Link(log))
    {
        OutputDebugStringA(("Failed to create shader program\n" + log + '\n').c_str());
        return false;
    }

    this->sphere.Create();
    this->sphere.Position = { 1, 0, 0 };

    this->triangle.Create();
    this->triangle.Position = { -1, 0, 0 };

    return true;
}