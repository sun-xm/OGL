#include "GLTexture.h"

GLTexture::GLTexture(GLenum target)
  : w(0), h(0), tex(0), target(target), envMode(GL_MODULATE), minFilter(GL_LINEAR), magFilter(GL_LINEAR), wrapS(GL_REPEAT), wrapT(GL_REPEAT)
{
}

void GLTexture::Release()
{
    if (this->tex)
    {
        glDeleteTextures(1, &this->tex);
        this->tex = 0;
    }
}

void GLTexture::Mode(GLuint envMode)
{
    this->envMode = envMode;
}

void GLTexture::Filter(GLuint minFilter, GLuint magFilter)
{
    this->minFilter = minFilter;
    this->magFilter = magFilter;
}

void GLTexture::Wrap(GLuint wrapS, GLuint wrapT)
{
    this->wrapS = wrapS;
    this->wrapT = wrapT;
}

bool GLTexture::Data(GLenum format, const unsigned char* pixels, size_t size, uint32_t width, uint32_t height)
{
    if (!pixels || !size || !width)
    {
        return false;
    }

    if (!this->tex && !this->Create())
    {
        return false;
    }

    GLint internalFormat;
    switch (format)
    {
        case GL_RGB:
        case GL_BGR:
        {
            internalFormat = GL_RGB;
            break;
        }

        case GL_RGBA:
        case GL_BGRA:
        {
            internalFormat = GL_RGBA;
            break;
        }

        default:
            return false;
    }

    glBindTexture(this->target, this->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (this->target)
    {
        case GL_TEXTURE_1D:
        {
            glTexImage1D(this->target, 0, internalFormat, width, 0, format, GL_UNSIGNED_BYTE, pixels);
            break;
        }

        case GL_TEXTURE_2D:
        {
            if (!height)
            {
                return false;
            }

            glTexImage2D(this->target, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
            break;
        }

        default:
            return false;
    }

    this->w = width;
    this->h = height;

    return true;
}

void GLTexture::Set() const
{
    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->envMode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->wrapS);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->wrapT);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->minFilter);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->magFilter);
    }
}

void GLTexture::Apply() const
{
    if (this->tex)
    {
        this->Set();
        glEnable(this->target);
    }
}

void GLTexture::Revoke() const
{
    if (this->tex)
    {
        glDisable(this->target);
        glBindTexture(this->target, 0);
    }
}

bool GLTexture::Create()
{
    if (!this->tex)
    {
        glGenTextures(1, &this->tex);
    }
    return !!this->tex;
}