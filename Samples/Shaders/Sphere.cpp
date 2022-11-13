#include "Sphere.h"
#include <cmath>

using namespace std;

#define RATIO  (4.f / sqrtf(6.f))
#define LEVELS (4)
#define RADIUS (0.5f)

Sphere::Sphere()
{
}

void Sphere::Create(shared_ptr<GLProgram>& program)
{
    this->program = program;

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

    this->Vertices(vertices.data(), vertices.size());

    auto ratio = 1.f / RADIUS;
    for (auto& v : vertices)
    {
        v *= ratio;
    }
    this->Normals(vertices.data(), vertices.size());

    vector<Vector<3>> colors(vertices.size(), { 1, 1, 1 });
    this->Colors(colors.data(), (int)colors.size());
}

void Sphere::Release()
{
    GLShape::Release();
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

void Sphere::Render(const GLScene& scene, const Vertex& lightPos)
{
    this->lightPos = lightPos;
    GLShape::Render(scene);
}

size_t Sphere::Apply(const GLScene& scene)
{
    auto count = this->vbo.Size() / sizeof(Vertex);
    if (count)
    {
        this->program->Use();

        this->program->UniformV3f("LightPos",  this->lightPos);
        this->program->UniformM4f("WorldView", scene.WorldView());

        Matrix<4> modelView, projection;
        glGetFloatv(GL_MODELVIEW_MATRIX,  modelView);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);
        this->program->UniformM4f("ModelView",  modelView,  false);
        this->program->UniformM4f("Projection", projection, false);

        this->program->BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
        this->program->BindAttrib("clr", this->cbo, 3, GL_FLOAT);
        this->program->BindAttrib("nml", this->nbo, 3, GL_FLOAT);
    }

    return count;
}

void Sphere::Revoke()
{
    this->program->UnbindAttrib("vtx");
    this->program->UnbindAttrib("clr");
    this->program->UnbindAttrib("nml");
    glUseProgram(0);
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