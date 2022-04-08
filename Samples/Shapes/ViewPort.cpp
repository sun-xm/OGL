#include "ViewPort.h"
#include "Texture.h"
#include "Triangle.h"
#include "Square.h"
#include "Plate.h"
#include "Cone.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Donut.h"
#include "resource.h"
#include <iostream>

ViewPort::ViewPort() : shape(nullptr)
{
}

void ViewPort::CreateShape(int shape)
{
    if (this->AttachContext())
    {
        GLShape* s = nullptr;

        switch (shape)
        {
            case IDC_TRIANGLE:
            {
                s = new Triangle();
                break;
            }

            case IDC_SQUARE:
            {
                s = new Square();
                break;
            }

            case IDC_PLATE:
            {
                s = new Plate();
                break;
            }

            case IDC_CUBE:
            {
                s = new Cube();
                break;
            }

            case IDC_CYLINDER:
            {
                s = new Cylinder();
                break;
            }

            case IDC_SPHERE:
            {
                s = new Sphere();
                break;
            }

            case IDC_DONUT:
            {
                s = new Donut();
                break;
            }

            case IDC_CONE:
            {
                s = new Cone();
                break;
            }

            default:
                break;
        }

        if (s)
        {
            Texture tex(L"Portrait.png");
            s->Texture().Data(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);

            if (this->shape)
            {
                this->shape->Release();
                delete this->shape;
            }
            this->shape = s;
            this->scene.Camera().Position(0.f, 0.f, 5.f);
            this->scene.Camera().LookAt(this->shape->Position);

            this->Invalidate();
        }

        this->DetachContext();
    }
}

bool ViewPort::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    auto& l0 = this->scene.EnableLight(0);
    l0.Ambient(1.f, 1.f, 1.f, 1.f);
    l0.Diffuse(1.f, 1.f, 1.f, 1.f);
    l0.Specular(1.f, 1.f, 1.f, 1.f);
    l0.Position(0.f, 0.f, 5.f, 1.f);

    auto& l1 = this->scene.EnableLight(1);
    l1.Diffuse({ 1.f, 1.f, 1.f, 1.f });
    l1.Specular(1.f, 1.f, 1.f, 1.f);
    l1.Position(0.f, 0.f, 5.f, 1.f);

    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().Focal(616.f);

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        if (MK_LBUTTON & this->wparam)
        {
            POINT pos;
            if (GetCursorPos(&pos))
            {
                auto x = pos.x - this->cursor.x;
                auto y = pos.y - this->cursor.y;
                this->cursor = pos;

                if (this->shape)
                {
                    auto qx = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
                    auto qy = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
                    auto qr = Quaternion<float>::FromRotation(this->shape->Rotation);

                    this->shape->Rotation = (qx * qy * qr).ToRotation();
                    this->Render();
                }
            }
        }
        return 0;
    });
    this->RegisterMessage(WM_LBUTTONDOWN, [this]
    {
        SetCapture(this->Handle());
        GetCursorPos(&this->cursor);
        return 0;
    });
    this->RegisterMessage(WM_LBUTTONUP, []
    {
        ReleaseCapture();
        return 0;
    });
    this->RegisterMessage(WM_LBUTTONDBLCLK, [this]
    {
        if (this->shape)
        {
            this->shape->Rotation = { 0.f, 0.f, 0.f, 0.f };
        }
        return 0;
    });
    this->RegisterMessage(WM_MOUSEWHEEL, [this]
    {
        auto delta = 1.f + (float)((short)HIWORD(this->wparam) / WHEEL_DELTA) * 0.05f;

        auto position = this->scene.Camera().Position();
        auto lookAt   = this->scene.Camera().LookAt();
        this->scene.Camera().Position((position - lookAt) * delta + lookAt);
        this->Render();
        return 0;
    });

    return true;
}

void ViewPort::OnContextDestroy()
{
    if (this->shape)
    {
        this->shape->Release();
        delete this->shape;
    }
    GLWindow::OnContextDestroy();
}

void ViewPort::OnPaint()
{
    this->Render();
    GLWindow::OnPaint();
}

void ViewPort::Render()
{
    if (this->AttachContext())
    {
        this->scene.Begin(this->ClientWidth(), this->ClientHeight());
        if (this->shape)
        {
            this->shape->Render(this->scene);
        }
        this->scene.End();

        this->SwapBuffers();
        this->DetachContext();
    }
}