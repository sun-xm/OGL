#pragma once

#include <gl/glew.h>
#include <cstdint>
#include <vector>

class GLTexture
{
public:
    GLTexture(GLenum target = GL_TEXTURE_2D);
    GLTexture(const GLTexture& other) = delete;

    bool Create();
    void Release();

    void Mode(GLuint envMode);
    void Wrap(GLuint wrapS, GLuint wrapT);
    void Filter(GLuint minFilter, GLuint magFilter);
    bool Data(GLenum format, const unsigned char* pixels, size_t size, uint32_t width, uint32_t height = 0);
    void Apply() const;
    void Revoke() const;

    GLuint Mode() const;
    GLuint WrapS() const;
    GLuint WrapT() const;
    GLuint MinFilter() const;
    GLuint MagFilter() const;

    GLuint Target() const { return  this->target; }
    uint32_t Width() const { return this->w; }
    uint32_t Height() const { return this->h; }

    operator bool() const { return !!this->tex; }
    operator GLuint() const { return this->tex; }

    GLTexture& operator=(const GLTexture& other) = delete;

private:
    GLuint  tex;
    GLenum  target;
    uint32_t w, h;
};