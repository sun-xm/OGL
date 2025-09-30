#include "Triangle.h"

using namespace std;
using namespace vx;

Triangle::Triangle()
{
    this->Position = {0};
    this->Rotation = {0};
}

void Triangle::Create()
{
    Vertex vertices[] = {{ -.5f, -.5f, 0.f }, { .5f, -.5f, 0.f }, { 0.f, .5f, 0.f }};
    this->vbo.Data(vertices, sizeof(vertices), GL_STATIC_DRAW);

    Vector<3> colors[] = {{ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }};
    this->cbo.Data(colors, sizeof(colors), GL_STATIC_DRAW);

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }};
    this->nbo.Data(normals, sizeof(normals), GL_STATIC_DRAW);
}

void Triangle::Release()
{
    this->vbo.Release();
    this->nbo.Release();
    this->cbo.Release();
}

bool Triangle::Colors(const Vector<3>* colors, int count)
{
    if (!colors || count <= 0)
    {
        return false;
    }

    if (!this->cbo.Data(colors, count * sizeof(colors[0]), GL_STATIC_DRAW))
    {
        return false;
    }

    return true;
}

void Triangle::Render(GLProgram& program, const Vertex& lightPos)
{
    program.UniformM4f("ModelView",  Transform3D<>::Shift(this->Position) * Quaternion<>::FromRotation(this->Rotation).ToMatrix());

    program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
    program.BindAttrib("clr", this->cbo, 3, GL_FLOAT);
    program.BindAttrib("nml", this->nbo, 3, GL_FLOAT);

    glDrawArrays(GL_TRIANGLES, 0, this->vbo.Size() / sizeof(Vertex));
}