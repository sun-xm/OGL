#include "Triangle.h"

Triangle::Triangle(Program& program) : program(program), cbo(GL_ARRAY_BUFFER)
{
}

void Triangle::Create()
{
    Vertex vertices[] = {{ -.5f, -.5f, 0.f }, { .5f, -.5f, 0.f }, { 0.f, .5f, 0.f }};
    this->Vertices(vertices, 3);

    Vector<float, 3> colors[] = {{ 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f }};
    this->Colors(colors, 3);

    Normal normals[] = {{ 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }};
    this->Normals(normals, 3);
}

void Triangle::Release()
{
    GLShape::Release();
    this->cbo.Release();
}

bool Triangle::Colors(const Vector<float, 3>* colors, int count)
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

void Triangle::Render(const GLScene& scene, const Vertex& lightPos)
{
    this->lightPos = lightPos;
    GLShape::Render(scene);
}

size_t Triangle::Apply(const GLScene& scene)
{
    auto count = this->vbo.Size() / sizeof(Vertex);
    if (count)
    {
        this->program.Use();

        this->program.UniformV3f("LightPos",  this->lightPos);
        this->program.UniformM4f("WorldView", scene.WorldView());

        Matrix<float, 4> modelView, projection;
        glGetFloatv(GL_MODELVIEW_MATRIX,  modelView);
        glGetFloatv(GL_PROJECTION_MATRIX, projection);
        this->program.UniformM4f("ModelView",  modelView,  false);
        this->program.UniformM4f("Projection", projection, false);

        this->program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
        this->program.BindAttrib("clr", this->cbo, 3, GL_FLOAT);
        this->program.BindAttrib("nml", this->nbo, 3, GL_FLOAT);
    }

    return count;
}

void Triangle::Revoke()
{
    this->program.UnbindAttrib("vtx");
    this->program.UnbindAttrib("clr");
    this->program.UnbindAttrib("nml");
    glUseProgram(0);
}