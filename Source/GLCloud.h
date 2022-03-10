#pragma once

#include "GLShape.h"
#include <vector>

class GLCloud : public GLShape
{
public:
    GLCloud();

    float PointSize() const;
    void  PointSize(float size);

    void  Render(const GLScene& scene) override;

protected:
    float pointSize;
};