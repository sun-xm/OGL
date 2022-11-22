#include "imgview.h"
#include <QFile>

using namespace std;

ImgView::ImgView(QWidget* parent) : QOpenGLWidget(parent)
{
}

ImgView::~ImgView()
{
    this->makeCurrent();
    this->vbo.Release();
    this->tbo.Release();
    this->tex.Release();
    this->program.Release();
}

void ImgView::initializeGL()
{
    if (GLEW_OK != glewInit())
    {
        cerr << "Failed to init glew" << endl;
        return;
    }

    QFile vsrc(":TexVtx.glsl");
    if (!vsrc.open(QIODevice::ReadOnly))
    {
        cerr << "Failed to open vertex source" << endl;
        return;
    }

    GLVShader vshader;
    if (!vshader.Source(vsrc.readAll().toStdString()) || !vshader.Compile())
    {
        cerr << "Failed to compile vertex source:" << endl << vshader.Log() << endl;
        return;
    }

    QFile fsrc(":TexFrg.glsl");
    if (!fsrc.open(QIODevice::ReadOnly))
    {
        cerr << "Failed to open fragment source" << endl;
        return;
    }

    GLFShader fshader;
    if (!fshader.Source(fsrc.readAll().toStdString()) || !fshader.Compile())
    {
        cerr << "Failed to compile fragment source: " << endl << fshader.Log() << endl;
        return;
    }

    if (!this->program.Link(vshader, fshader))
    {
        cerr << "Failed to line program: " << this->program.Log() << endl;
        return;
    }

    QImage img;
    if (!img.load(":Portrait.png"))
    {
        cerr << "Failed to load texture" << endl;
        return;
    }

    if (!this->tex.Data(GL_BGRA, (uint32_t*)img.constBits(), img.width(), img.height()))
    {
        cerr << "Failed to set texture data" << endl;
        return;
    }
    this->tex.Wrap(GL_CLAMP, GL_CLAMP);
    this->tex.Filter(GL_LINEAR, GL_LINEAR);

    auto w = (float)img.width();
    auto h = (float)img.height();

    Coordinate coords[] = {{ 0, 0 }, { w, 0 }, { w, h },
                           { 0, 0 }, { 0, h }, { w, h }};
    Vertex vertices[COUNTOF(coords)];
    for (size_t i = 0; i < COUNTOF(coords); i++)
    {
        auto& c = coords[i];
        vertices[i] = { c.X, -c.Y, 1 };
    }
    if (!this->vbo.Data(vertices, sizeof(vertices), GL_STATIC_DRAW))
    {
        cerr << "Failed to set vertex data" << endl;
        return;
    }

    Coordinate texmap[] = {{ 0, 0 }, { 1, 0 }, { 1, 1 },
                           { 0, 0 }, { 0, 1 }, { 1, 1 }};
    if (!this->tbo.Data(texmap, sizeof(texmap), GL_STATIC_DRAW))
    {
        cerr << "Failed to set texture coodinates" << endl;
        return;
    }

    QOpenGLWidget::initializeGL();
}

void ImgView::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
    glViewport(0, 0, w, h);
    this->unify  = Transform2D<>::Scale(2.f / w, 2.f / h);
    this->origin = Transform2D<>::Shift(-w * .5f, h * .5f);
}

void ImgView::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    auto x = this->size().width()  - (int)this->tex.Width();
    auto y = this->size().height() - (int)this->tex.Height();
    auto t = Transform2D<>::Shift(x * 0.5f, -y * 0.5f);

    this->program.Use();
    this->program.BindAttrib("vtx", this->vbo, 3, GL_FLOAT);
    this->program.BindAttrib("crd", this->tbo, 2, GL_FLOAT);
    this->program.UniformTex("tex", this->tex, 0);
    this->program.UniformM3f("Matrix", this->unify * this->origin * t);

    glDrawArrays(GL_TRIANGLES, 0, this->vbo.Size() / sizeof(Vertex));
    glUseProgram(0);
}