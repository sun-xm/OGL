#include "Sphere.h"
#include <cmath>

using namespace std;
using namespace vx;

#define RATIO  (4.f / sqrtf(6.f))
#define LEVELS (4)
#define RADIUS (0.5f)

Sphere::Sphere()
{
    this->Position = {0};
    this->Rotation = {0};
}

void Sphere::Create()
{
    vector<Vertex> vertices;

    Vertex v0 = { RADIUS, 0, 0 };
    Vertex v1 = { 0, 0, RADIUS };
    Vertex v2 = { 0, RADIUS, 0 };

    // Split 8 faces of an octahedron
    Split(1,  v0,  v1, v2, vertices);
    Split(1,  v0, -v1, v2, vertices);
    Split(1, -v0,  v1, v2, vertices);
    Split(1, -v0, -v1, v2, vertices);

    Split(1,  v0,  v1, -v2, vertices);
    Split(1,  v0, -v1, -v2, vertices);
    Split(1, -v0,  v1, -v2, vertices);
    Split(1, -v0, -v1, -v2, vertices);

    this->vbo.Data(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_STATIC_DRAW);

    auto ratio = 1.f / RADIUS;
    for (auto& v : vertices)
    {
        v *= ratio;
    }
    this->nbo.Data(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_STATIC_DRAW);

    vector<Vector<3>> colors(vertices.size(), { 1, 1, 1 });
    this->cbo.Data(colors.data(), colors.size() * sizeof(colors[0]), GL_STATIC_DRAW);
}

void Sphere::Release()
{
    this->vbo.Release();
    this->nbo.Release();
    this->cbo.Release();
}

bool Sphere::Colors(const Vector<3>* colors, int count)
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

void Sphere::Render(GLProgram& program, const Vertex& lightPos)
{
    program.UniformM4f("ModelView",  Transform3D<>::Shift(this->Position) * Quaternion<>::FromRotation(this->Rotation).ToMatrix());

    program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
    program.BindAttrib("clr", this->cbo, 3, GL_FLOAT);
    program.BindAttrib("nml", this->nbo, 3, GL_FLOAT);

    glDrawArrays(GL_TRIANGLES, 0, this->vbo.Size() / sizeof(Vertex));
}

void Sphere::Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, vector<Vertex>& vertices)
{
    if (level > LEVELS)
    {
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        return;
    }

    Vertex v[] = { (v0 + v1).Normalize() * RADIUS,
                   (v1 + v2).Normalize() * RADIUS,
                   (v2 + v0).Normalize() * RADIUS };

    Split(level + 1, v0, v[0], v[2], vertices);
    Split(level + 1, v1, v[1], v[0], vertices);
    Split(level + 1, v2, v[2], v[1], vertices);

    Split(level + 1, v[0], v[1], v[2], vertices);
}