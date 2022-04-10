#pragma once

#include "GLWindow.h"
#include "GLBuffer.h"
#include "GLProgram.h"
#include "GLShader.h"
#include "GLScene.h"
#include "GLTexture.h"

class Viewport : public GLWindow
{
public:
    Viewport();

protected:
    bool OnContextCreated() override;
    void OnContextDestroy() override;
    void OnTimer() override;
    void OnPaint() override;

private:
    void Render(float x, float y, float w, float h);

private:
    GLScene   scene;

    GLShader  vshader;
    GLShader  fshader;
    GLProgram program;

    GLBuffer  vbo;
    GLBuffer  tbo;
    GLTexture tex;

    float rotate;
};