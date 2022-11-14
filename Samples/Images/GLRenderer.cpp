#include "GLRenderer.h"
#include "Cleanup.h"
#include <gl/glew.h>
#include <fstream>

using namespace std;

GLRenderer::GLRenderer() : transform(Transform2D<>::Identity())
{
}

bool GLRenderer::Create(string& log)
{
    if (!this->texProg.Create() || !this->clrProg.Create())
    {
        return false;
    }

    GLVShader vshader;
    GLFShader fshader;
    ONCLEANUP(vshader, [&]{ vshader.Release(); });
    ONCLEANUP(fshader, [&]{ fshader.Release(); });

    ifstream texVtx(L"TexVtx.glsl");
    ifstream texFrg(L"TexFrg.glsl");
    if (!texVtx.is_open() ||
        !texVtx.is_open())
    {
        return false;
    }

    if (!vshader.Source(texVtx) || !vshader.Compile())
    {
        log = vshader.Log();
        return false;
    }

    if (!fshader.Source(texFrg) || !fshader.Compile())
    {
        log = fshader.Log();
        return false;
    }

    if (!this->texProg.Link(vshader, fshader))
    {
        log = this->texProg.Log();
        return false;
    }

    vshader.Release();
    fshader.Release();

    ifstream clrVtx(L"ClrVtx.glsl");
    ifstream clrFrg(L"ClrFrg.glsl");
    if (!clrVtx.is_open() ||
        !clrFrg.is_open())
    {
        return false;
    }

    if (!vshader.Source(clrVtx) || !vshader.Compile())
    {
        log = vshader.Log();
        return false;
    }

    if (!fshader.Source(clrFrg) || !fshader.Compile())
    {
        log = fshader.Log();
        return false;
    }

    if (!this->clrProg.Link(vshader, fshader))
    {
        log = this->clrProg.Log();
        return false;
    }

    return true;
}

void GLRenderer::Release()
{
    this->texProg.Release();
    this->clrProg.Release();
}

void GLRenderer::Begin(int w, int h)
{
    this->origin = Transform2D<>::Shift(-w * .5f, h * .5f);    // Shift origin of coordinates to upper-left corner
    this->unify  = Transform2D<>::Scale(2.f / w,  2.f / h);    // Unify projected coordinates to -1 ~ +1

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::End()
{
    glFlush();
}

void GLRenderer::Identity()
{
    this->transform = Transform2D<>::Identity();
}

void GLRenderer::Rotate(float x, float y, float radian)
{
    this->transform = Transform2D<>::Rotate(x, -y, radian) * this->transform;
}

void GLRenderer::Scale(float x, float y)
{
    this->transform = Transform2D<>::Scale(x, y) * this->transform;
}

void GLRenderer::Shift(float x, float y)
{
    this->transform = Transform2D<>::Shift(x, -y) * this->transform;
}

void GLRenderer::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLRenderer::Triangles(const Coordinate* coordinates, size_t count, const Vector<4>& color)
{
    vector<Vertex> vertices(count);
    for (size_t i = 0; i < count; i++)
    {
        auto& c = coordinates[i];
        vertices[i] = Vertex{ c.X, -c.Y, 1 };
    }

    GLBuffer vbo;
    ONCLEANUP(vbo, [&]{ vbo.Release(); });

    if (!vbo.Data(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_DYNAMIC_DRAW))
    {
        return;
    }

    this->clrProg.Use();
    this->clrProg.BindAttrib("vtx", vbo, 3, GL_FLOAT);
    this->clrProg.UniformV4f("Color", color);
    this->clrProg.UniformM3f("Matrix", this->unify * this->origin * this->transform);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

    this->clrProg.UnbindAttrib("vtx");
    glUseProgram(0);
}

void GLRenderer::Triangles(const Coordinate* coordinates, const Coordinate* texCoords, size_t count, const GLTexture& texture)
{
    vector<Vertex> vertices(count);
    for (size_t i = 0; i < count; i++)
    {
        auto& c = coordinates[i];
        vertices[i] = Vertex{ c.X, -c.Y, 1 };
    }

    GLBuffer vbo, tbo;
    ONCLEANUP(buffers, [&]{ vbo.Release(); tbo.Release(); });

    if (!vbo.Data(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_DYNAMIC_DRAW) ||
        !tbo.Data(texCoords, count * sizeof(texCoords[0]), GL_DYNAMIC_DRAW))
    {
        return;
    }

    this->texProg.Use();
    this->texProg.BindAttrib("vtx", vbo, 3, GL_FLOAT);
    this->texProg.BindAttrib("crd", tbo, 2, GL_FLOAT);
    this->texProg.UniformM3f("Matrix", this->unify * this->origin * this->transform);
    this->texProg.UniformTex("tex", texture, 0);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

    this->texProg.UnbindAttrib("vtx");
    this->texProg.UnbindAttrib("crd");
    glUseProgram(0);
}

void GLRenderer::Image(const GLTexture& texture, float w, float h)
{
    Coordinate coords[] = {{ 0, 0 }, { w, 0 }, { 0, h },
                           { 0, h }, { w, h }, { w, 0 }};
    Coordinate texCrd[] = {{ 0, 0 }, { 1, 0 }, { 0, 1 },
                           { 0, 1 }, { 1, 1 }, { 1, 0 }};
    this->Triangles(coords, texCrd, 6, texture);
}

void GLRenderer::Line(float x0, float y0, float x1, float y1)
{
    Vertex v0 = { x0, -y0, 1 };
    Vertex v1 = { x1, -y1, 1 };

    auto m = this->unify * this->origin * this->transform;
    v0 = m * v0;
    v1 = m * v1;

    glBegin(GL_LINES);
    glVertex2f(v0.X, v0.Y);
    glVertex2f(v1.X, v1.Y);
    glEnd();
}