#include "Viewport.h"
#include "Texture.h"

using namespace std;

Viewport::Viewport() : vbo(GL_ARRAY_BUFFER), tbo(GL_ARRAY_BUFFER)
{
}

bool Viewport::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    if (!this->program.Create())
    {
        return false;
    }

    string log;
    if (!this->vshader.Create(GL_VERTEX_SHADER)   || !this->vshader.Load(L"VertexShader.glsl")   || !this->vshader.Compile(log) ||
        !this->fshader.Create(GL_FRAGMENT_SHADER) || !this->fshader.Load(L"FragmentShader.glsl") || !this->fshader.Compile(log))
    {
        OutputDebugStringA(("Failed to create shaders:\n" + log + '\n').c_str());
        return false;
    }

    this->program.Attach(this->vshader);
    this->program.Attach(this->fshader);

    if (!this->program.Link(log))
    {
        OutputDebugStringA(("Failed to link program:\n" + log + '\n').c_str());
        return false;
    }

    Texture tex(L"Portrait.png");
    this->tex.Data(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);

    return true;
}

void Viewport::OnContextDestroy()
{
    this->vshader.Release();
    this->fshader.Release();
    this->program.Release();

    this->vbo.Release();
    this->tbo.Release();
    this->tex.Release();

    GLWindow::OnContextDestroy();
}

void Viewport::OnPaint()
{
    auto w = this->ClientWidth();
    auto h = this->ClientHeight();

    if (this->AttachContext())
    {
        glClearColor(1, 1, 1, 1);
        this->scene.Begin(w, h);
        this->Render((w - 400) / 2.f, (h - 400) / 2.f, 400, 400);
        this->scene.End();
        this->DetachContext();
    }

    GLWindow::OnPaint();
}

void Viewport::Render(float x, float y, float w, float h)
{
    auto l = x / this->scene.Width()  * 2 - 1;
    auto t = y / this->scene.Height() * 2 - 1;
    auto r = (x + w) / this->scene.Width()  * 2 - 1;
    auto b = (y + h) / this->scene.Height() * 2 - 1;

    vector<Vertex> vertices = {{ l, t, 0 }, { r, t, 0 }, { l, b, 0 },
                               { l, b, 0 }, { r, b, 0 }, { r, t, 0 }};
    this->vbo.Data(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_DYNAMIC_DRAW);

    vector<Coordinate> coords = {{ 0, 1 }, { 1, 1 }, { 0, 0 },
                                 { 0, 0 }, { 1, 0 }, { 1, 1 }};
    this->tbo.Data(coords.data(), coords.size() * sizeof(coords[0]), GL_DYNAMIC_DRAW);

    this->program.Use();
    this->program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
    this->program.BindAttrib("crd", this->tbo, 2, GL_FLOAT);

    this->tex.Apply();

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

    this->tex.Revoke();

    this->program.UnbindAttrib("vtx");
    this->program.UnbindAttrib("crd");
    glUseProgram(0);
}