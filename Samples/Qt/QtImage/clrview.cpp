#include "clrview.h"

using namespace std;

ClrView::ClrView(QWidget* parent) : GLView(parent), program(nullptr), vbo(0), w(100), h(100)
{
}

ClrView::~ClrView()
{
    delete this->program;

    if (this->vbo)
    {
        glDeleteBuffers(1, &vbo);
    }
}

void ClrView::initializeGL()
{
    GLView::initializeGL();

    this->program = new QOpenGLShaderProgram(this);
    this->program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":ClrVtx.glsl");
    this->program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":ClrFrg.glsl");
    this->program->link();

    glGenBuffers(1, &this->vbo);
    this->Coordinates({{ 0, 0 }, { this->w, 0 }, { this->w, this->h },
                       { 0, 0 }, { 0, this->w }, { this->w, this->h }},
                       this->vbo, GL_STATIC_DRAW);
}

void ClrView::resizeGL(int w, int h)
{
    GLView::resizeGL(w, h);

    this->transform = Transform2D<>::Identity();
    this->transform = Transform2D<>::Shift((w - this->w) * 0.5f, -(h - this->h)* 0.5f) * this->transform;
}

void ClrView::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    this->program->bind();
    this->BindAttrib(this->program->programId(), "vtx", this->vbo, 3, GL_FLOAT);

    Vector<4> color = { 1, 1, 1, 1 };
    this->UniformV4f(this->program->programId(), "Color", color);
    this->UniformM3f(this->program->programId(), "Matrix", this->unify * this->origin * this->transform);

    auto size = this->BufferSize(this->vbo);

    glDrawArrays(GL_TRIANGLES, 0, this->BufferSize(this->vbo) / sizeof(Vertex));

    this->UnbindAttrib(this->program->programId(), "vtx");
}