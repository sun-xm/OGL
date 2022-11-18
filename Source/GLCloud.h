#pragma once

#include "GLShape.h"
#include <vector>

class GLCloud : public GLShape
{
public:
    GLCloud() : pointSize(1.f)
    {
        this->mode = GL_POINTS;
    }
    virtual ~GLCloud() {}

    float PointSize() const
    {
        return this->pointSize;
    }
    void  PointSize(float size)
    {
        this->pointSize = size;
    }

    void  Render(const GLScene& scene) override
    {
        glPointSize(this->pointSize);
        GLShape::Render(scene);
    }

protected:
    float pointSize;
};