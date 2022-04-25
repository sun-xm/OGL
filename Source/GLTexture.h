#pragma once

#include <gl/glew.h>
#include <vector>

class GLTexture
{
public:
    GLTexture(GLenum target = GL_TEXTURE_2D);

    void Mode(GLuint envMode);
    void Filter(GLuint minFilter, GLuint magFilter);
    void Wrap(GLuint wrapS, GLuint wrapT);
    bool Data(GLenum format, const unsigned char* pixels, size_t size, uint32_t width, uint32_t height = 0);
    void Set() const;
    void Apply() const;
    void Revoke() const;
    void Release();

    GLuint Mode() const { return this->envMode; }
    GLuint MinFilter() const { return this->minFilter; }
    GLuint MagFilter() const { return this->magFilter; }
    GLuint WrapS() const { return this->wrapS; }
    GLuint WrapT() const { return this->wrapT; }
    GLuint Target() const { return  this->target; }
    uint32_t Width() const { return this->w; }
    uint32_t Height() const { return this->h; }

    operator bool() const { return !!this->tex; }
    operator GLuint() const { return this->tex; }

private:
    bool Create();

private:
    GLuint envMode;
    GLuint minFilter;
    GLuint magFilter;
    GLuint wrapS;
    GLuint wrapT;

    GLuint tex;
    GLenum target;

    uint32_t w, h;
};