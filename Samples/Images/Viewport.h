#pragma once

#include "GLWindow.h"
#include "GLBuffer.h"
#include "GLProgram.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLRenderer.h"

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
    GLRenderer renderer;
    GLTexture  texture;

    float rotate;
};