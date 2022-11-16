#include "GLTexture.h"

GLTexture::GLTexture(GLenum target)
  : w(0), h(0), tex(0), target(target)
{
}

bool GLTexture::Create()
{
    if (!this->tex)
    {
        glGenTextures(1, &this->tex);
    }

    return !!this->tex;
}

void GLTexture::Release()
{
    glDeleteTextures(1, &this->tex);
    this->tex = 0;
}

bool GLTexture::Mode(GLuint envMode)
{
    if (!this->tex)
    {
        return false;
    }

    glBindTexture(this->target, this->tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envMode);

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

bool GLTexture::Wrap(GLuint wrapS, GLuint wrapT)
{
    if (!this->tex)
    {
        return false;
    }

    glBindTexture(this->target, this->tex);
    glTexParameteri(this->target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(this->target, GL_TEXTURE_WRAP_T, wrapT);

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

bool GLTexture::Filter(GLuint minFilter, GLuint magFilter)
{
    if (!this->tex)
    {
        return false;
    }

    glBindTexture(this->target, this->tex);
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, magFilter);

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

bool GLTexture::Data(GLenum format, const uint32_t* pixels, uint32_t width, uint32_t height)
{
    if (!pixels || !width || (GL_TEXTURE_2D == this->target && !height))
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
            glTexImage2D(this->target, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
            break;
        }

        default:
            return false;
    }

    auto err = glGetError();
    if (GL_NO_ERROR != err)
    {
        return false;
    }

    this->w = width;
    this->h = height;

    return true;
}

bool GLTexture::Data(const uint16_t* pixels, uint32_t width, uint32_t height)
{
    if (!pixels || !width || (GL_TEXTURE_2D == this->target && !height))
    {
        return false;
    }

    if (!this->tex && !this->Create())
    {
        return false;
    }

    glBindTexture(this->target, this->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (this->target)
    {
        case GL_TEXTURE_1D:
        {
            glTexImage1D(this->target, 0, GL_R16UI, width, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, pixels);
            break;
        }

        case GL_TEXTURE_2D:
        {
            glTexImage2D(this->target, 0, GL_R16UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, pixels);
            break;
        }

        default:
            return false;
    }

    auto err = glGetError();
    if (GL_NO_ERROR != err)
    {
        return false;
    }

    this->w = width;
    this->h = height;

    return true;
}

bool GLTexture::Apply() const
{
    if (!this->tex)
    {
        return false;
    }

    glBindTexture(this->target, this->tex);
    glEnable(this->target);

    auto err = glGetError();
    return GL_NO_ERROR == err;
}

void GLTexture::Revoke() const
{
    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glDisable(this->target);
    }
}

GLuint GLTexture::Mode() const
{
    GLuint mode = 0;

    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint*)&mode);
    }

    return mode;
}

GLuint GLTexture::WrapS() const
{
    GLuint wrapS = 0;

    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glGetTexParameteriv(this->target, GL_TEXTURE_WRAP_S, (GLint*)&wrapS);
    }

    return wrapS;
}

GLuint GLTexture::WrapT() const
{
    GLuint wrapT = 0;

    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glGetTexParameteriv(this->target, GL_TEXTURE_WRAP_T, (GLint*)&wrapT);
    }

    return wrapT;
}

GLuint GLTexture::MinFilter() const
{
    GLuint minFilter = 0;

    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glGetTexParameteriv(this->target, GL_TEXTURE_MIN_FILTER, (GLint*)&minFilter);
    }

    return minFilter;
}

GLuint GLTexture::MagFilter() const
{
    GLuint magFilter = 0;

    if (this->tex)
    {
        glBindTexture(this->target, this->tex);
        glGetTexParameteriv(this->target, GL_TEXTURE_MAG_FILTER, (GLint*)&magFilter);
    }

    return magFilter;
}