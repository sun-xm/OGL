#include "imgview.h"

using namespace std;

ImgView::ImgView(QWidget* parent) : GLView(parent), program(nullptr), vbo(0), tbo(0), tex(0)
{
}

ImgView::~ImgView()
{
    delete this->program;

    if (vbo)
    {
        glDeleteBuffers(1, &this->vbo);
    }
    if (tbo)
    {
        glDeleteBuffers(1, &this->tbo);
    }
    if (tex)
    {
        glDeleteTextures(1, &this->tex);
    }
}

void ImgView::initializeGL()
{
    GLView::initializeGL();

    this->program = new QOpenGLShaderProgram();
    this->program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":TexVtx.glsl");
    this->program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":TexFrg.glsl");
    this->program->link();

    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->tbo);

    Coordinate texmap[] = {{ 0, 0 }, { 1, 0 }, { 1, 1 },
                           { 0, 0 }, { 0, 1 }, { 1, 1 }};
    glBindBuffer(GL_ARRAY_BUFFER, this->tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texmap), texmap, GL_STATIC_DRAW);

    QImage img;
    img.load(":Portrait.png");
    glGenTextures(1, &this->tex);
    glBindTexture(GL_TEXTURE_2D, this->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, img.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto w = (float)img.width();
    auto h = (float)img.height();
    vector<Coordinate> coords = {{ 0, 0 }, { w, 0 }, { w, h },
                                 { 0, 0 }, { 0, h }, { w, h }};
    this->Coordinates(coords, this->vbo, GL_STATIC_DRAW);
}

void ImgView::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);

    this->program->bind();
    this->BindAttrib(this->program->programId(), "vtx", this->vbo, 3, GL_FLOAT);
    this->BindAttrib(this->program->programId(), "crd", this->tbo, 2, GL_FLOAT);
    this->UniformM3f(this->program->programId(), "Matrix", this->unify * this->origin);
    this->UniformTex(this->program->programId(), "tex", this->tex, GL_TEXTURE_2D, 0);

    glDrawArrays(GL_TRIANGLES, 0, this->BufferSize(this->vbo) / sizeof(Vertex));
}