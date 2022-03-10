#include "Sphere.h"
#include <cmath>

using namespace std;

#define RATIO (4.f / sqrtf(6.f))

const Vertex Sphere::RegularTetrahedron[] = {{  0.f, 1.f, 0.f },
                                             {  0.f, -1.f / 3.f, 4.f / sqrtf(18.f) },
                                             {  .5f * RATIO, -1.f / 3.f, -2.f / sqrtf(18.f)},
                                             { -.5f * RATIO, -1.f / 3.f, -2.f / sqrtf(18.f)}};

Sphere::Sphere(Program& program) : program(program), cbo(GL_ARRAY_BUFFER)
{
}

void Sphere::Create()
{
    this->recursion = 5;

    vector<Vertex> vertices;
    vector<Normal> normals;

    int c = COUNTOF(RegularTetrahedron);

    for (auto i = 0; i < this->recursion; i++)
    {
        c *= COUNTOF(RegularTetrahedron);
    }

    vertices.reserve(c);
    normals.reserve(c);

    this->Split(.5f, 1, RegularTetrahedron[0], RegularTetrahedron[1], RegularTetrahedron[2], vertices, normals);
    this->Split(.5f, 1, RegularTetrahedron[0], RegularTetrahedron[3], RegularTetrahedron[1], vertices, normals);
    this->Split(.5f, 1, RegularTetrahedron[1], RegularTetrahedron[3], RegularTetrahedron[2], vertices, normals);
    this->Split(.5f, 1, RegularTetrahedron[0], RegularTetrahedron[2], RegularTetrahedron[3], vertices, normals);

    this->Vertices(vertices.data(), (int)vertices.size());
    this->Normals(normals.data(), (int)normals.size());

    vector<Vector<float, 3>> colors(vertices.size(), { 1, 1, 1 });
    this->Colors(colors.data(), (int)colors.size());
}

void Sphere::Release()
{
    GLShape::Release();
    this->cbo.Release();
}

bool Sphere::Colors(const Vector<float, 3>* colors, int count)
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

void Sphere::Split(float radius, int recursion, const Vertex& v0, const Vertex& v1, const Vertex& v2, vector<Vertex>& vertices, vector<Normal>& normals)
{
    if (recursion > this->recursion)
    {
        vertices.push_back(v0 * radius);
        vertices.push_back(v1 * radius);
        vertices.push_back(v2 * radius);

        normals.push_back(v0);
        normals.push_back(v1);
        normals.push_back(v2);

        return;
    }

    Vertex mid[] = { (v0 + v1).Normalize(), (v1 + v2).Normalize(), (v2 + v0).Normalize() };

    recursion++;
    this->Split(radius, recursion, mid[2], v0, mid[0], vertices, normals);
    this->Split(radius, recursion, mid[0], v1, mid[1], vertices, normals);
    this->Split(radius, recursion, mid[1], v2, mid[2], vertices, normals);
    this->Split(radius, recursion, mid[0], mid[1], mid[2], vertices, normals);
}

size_t Sphere::Apply(const GLScene& scene)
{
    auto loc = this->program.GetAttribLocation("nml");

    auto count = this->vbo.Size() / sizeof(Vertex);
    if (count)
    {
        this->program.Use();

        this->program.UniformV3f("LightPos",  this->lightPos);
        this->program.UniformM4f("WorldView", scene.WorldView());

        Matrix<float, 4> modelView, projection;
        glGetFloatv(GL_MODELVIEW_MATRIX,  modelView);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);
        this->program.UniformM4f("ModelView",  modelView);
        this->program.UniformM4f("Projection", projection);

        this->program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
        this->program.BindAttrib("clr", this->cbo, 3, GL_FLOAT);
        this->program.BindAttrib("nml", this->nbo, 3, GL_FLOAT);
    }

    return count;
}

void Sphere::Revoke()
{
    this->program.UnbindAttrib("vtx");
    this->program.UnbindAttrib("clr");
    this->program.UnbindAttrib("nml");
    glUseProgram(0);
}