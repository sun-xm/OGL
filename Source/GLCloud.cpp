#include "GLCloud.h"

GLCloud::GLCloud() : pointSize(1.f)
{
    this->mode = GL_POINTS;
}

float GLCloud::PointSize() const
{
    return this->pointSize;
}

void GLCloud::PointSize(float size)
{
    this->pointSize = size;
}

void GLCloud::Render(const GLScene& scene)
{
    glPointSize(this->pointSize);
    GLShape::Render(scene);
}