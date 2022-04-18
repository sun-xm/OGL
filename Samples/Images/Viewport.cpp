#include "Viewport.h"
#include "Texture.h"

using namespace std;

Viewport::Viewport() : rotate(0)
{
}

bool Viewport::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    string log;
    if (!this->renderer.Create(log))
    {
        OutputDebugStringA(("Failed to create renderer:\n" + log + '\n').c_str());
        return false;
    }

    Texture tex(L"Portrait.png");
    this->texture.Data(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);

    this->SetTimer(0, 17);

    return true;
}

void Viewport::OnContextDestroy()
{
    this->renderer.Release();
    this->texture.Release();

    GLWindow::OnContextDestroy();
}

void Viewport::OnTimer()
{
    this->rotate += .5f;
    if (this->rotate > 360.f)
    {
        this->rotate -= 360.f;
    }

    this->Invalidate();
}

void Viewport::OnPaint()
{
    auto w = this->ClientWidth();
    auto h = this->ClientHeight();

    if (this->AttachContext())
    {
        this->renderer.Begin(w, h);
        this->renderer.Clear(1, 1, 1, 1);
        this->renderer.Identity();
        this->renderer.Rotate(200, 200, ToRadian(this->rotate));
        this->renderer.Shift((w - 400) * .5f, (h - 400) * .5f);
        this->renderer.Image(this->texture, 400, 400);

        glColor3f(1, 0, 0);
        glLineWidth(2);
        this->renderer.Line(0, 0, 400, 400);
        this->renderer.End();
        this->SwapBuffers();
        this->DetachContext();
    }

    GLWindow::OnPaint();
}