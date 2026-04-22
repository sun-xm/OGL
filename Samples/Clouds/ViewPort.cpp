#include "ViewPort.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

ViewPort::ViewPort()
{
}

LRESULT ViewPort::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOUSEMOVE:
        if (MK_LBUTTON & wParam)
        {
            POINT cursor;
            if (GetCursorPos(&cursor))
            {
                auto x = cursor.x - this->cursor.x;
                auto y = this->cursor.y - cursor.y;
                this->cursor = cursor;

                auto qx = Quaternion<>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
                auto qy = Quaternion<>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
                auto qr = Quaternion<>::FromRotation(this->cloud.Rotation);

                this->cloud.Rotation = (qx * qy * qr).ToRotation();
                this->Invalidate();
            }
        }
        break;

    case WM_LBUTTONDOWN:
        SetCapture(*this);
        GetCursorPos(&this->cursor);
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        break;

    case WM_LBUTTONDBLCLK:
        this->cloud.Rotation = { 0.f, 0.f, 0.f, 0.f };
        this->Invalidate();
        break;

    case WM_MOUSEWHEEL:
        {
            auto delta = (short)HIWORD(wParam) / WHEEL_DELTA;
            this->scene.Camera().Position(this->scene.Camera().Position() * (1.f + delta * 0.05f));
            this->Invalidate();
        }

        break;

    default:
        break;
    }

    return GLWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}

bool ViewPort::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().LookAt(0.f, 0.f, 0.f);
    this->scene.Camera().Position(0.f, 0.f, -2.f);

    GLCShader center;
    if (!center.Source(ifstream("center.glsl")) || !center.Compile())
    {
        cerr << "Failed to load shader center:" << endl << center.Log() << endl;
        return false;
    }

    if (!this->center.Link(center))
    {
        cerr << "Failed to link program center:" << endl << this->center.Log() << endl;
        return false;
    }

    GLCShader shift;
    if (!shift.Source(ifstream("shift.glsl")) || !shift.Compile())
    {
        cerr << "Failed to load shader shift:" << endl << shift.Log() << endl;
        return false;
    }

    if (!this->shift.Link(shift))
    {
        cerr << "Failed to link program shift:" << endl << this->shift.Log() << endl;
        return false;
    }

    if (!this->LoadCloud())
    {
        return false;
    }

    return true;
}

void ViewPort::OnContextDestroy()
{
    this->cloud.Release();
    this->center.Release();
    this->shift.Release();
    GLWindow::OnContextDestroy();
}

void ViewPort::OnPaint()
{
    if (this->AttachContext())
    {
        this->scene.Begin(this->ClientW(), this->ClientH());
        this->cloud.Render(this->scene);
        this->scene.End();

        this->SwapBuffers();
        this->DetachContext();
    }

    GLWindow::OnPaint();
}

bool ViewPort::LoadCloud()
{
    ifstream ifs("cloud.txt");
    if (!ifs)
    {
        return false;
    }

    vector<float> points;
    float value;

    while (!ifs.eof())
    {
        ifs >> value;
        points.push_back(value);
    }

    points.resize((points.size() / 3) * 3);
    if (!points.size())
    {
        cerr << "Invalid points size" << endl;
        return false;
    }

    GLBuffer positions(GL_SHADER_STORAGE_BUFFER);
    if (!positions.Data(points.data(), points.size() * sizeof(points[0]), GL_DYNAMIC_DRAW))
    {
        cerr << "Failed to upload points data" << endl;
        return false;
    }

    static const size_t local = 256;
    auto groups = (points.size() / 3 + local - 1) / local;

    GLBuffer summary(GL_SHADER_STORAGE_BUFFER);
    if (!summary.Data(&Vector<3>::Zero, sizeof(Vector<3>), GL_DYNAMIC_DRAW))
    {
        cerr << "Failed to initialize summary buffer" << endl;
        return false;
    }

    GLBuffer nancnt(GL_ATOMIC_COUNTER_BUFFER);
    if (!nancnt.Data(Vector<1, uint32_t>::Zero, sizeof(uint32_t), GL_DYNAMIC_DRAW))
    {
        cerr << "Failed to initialize nancnt buffer" << endl;
        return false;
    }

    if (!this->center.Use())
    {
        cerr << "Failed use program" << endl;
        return false;
    }

    if (!this->center.BindBuffer("Position", positions) ||
        !this->center.BindBuffer("Summary", summary) ||
        !this->center.BindUniformAtomic("nancnt", nancnt))
    {
        cerr << "Failed to bind center buffers" << endl;
        return false;
    }

    glDispatchCompute((GLuint)groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    auto err = glGetError();
    if (GL_NO_ERROR != err)
    {
        cerr << "Failed to execute shader center: " << err << endl;
        return false;
    }

    Vector<3> sum;
    uint32_t  cnt;
    if (!summary.Read(&sum, sizeof(sum)) ||
        !nancnt.Read(&cnt, sizeof(cnt)))
    {
        cerr << "Failed to read summary or count buffer" << endl;
        return false;
    }

    if (cnt == points.size())
    {
        cerr << "No valid points" << endl;
        return false;
    }

    sum /= (float)(points.size() / 3 - cnt);
    summary.Data(&sum, sizeof(sum), GL_STATIC_DRAW);

    this->shift.Use();
    if (!this->shift.BindBuffer("Position", positions) ||
        !this->shift.BindBuffer("Center", summary))
    {
        cerr << "Failed to bind shift buffers" << endl;
        return false;
    }

    glDispatchCompute((GLuint)groups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    err = glGetError();
    if (GL_NO_ERROR != err)
    {
        cerr << "Failed to execute shader shift: " << err << endl;
        return false;
    }

    this->cloud.Vertices().Copy(positions);

    return true;
}