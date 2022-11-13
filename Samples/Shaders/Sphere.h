#pragma once

#include "GLShape.h"
#include "GLProgram.h"
#include <memory>
#include <vector>

class Sphere : public GLShape
{
public:
    Sphere();

    void Create(std::shared_ptr<GLProgram>& program);
    void Release() override;

    bool Colors(const Vector<3>* colors, int count);
    void Render(const GLScene& scene, const Vertex& lightPos);

protected:
    size_t Apply(const GLScene& scene) override;
    void   Revoke() override;

private:
    static void Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices);

    std::shared_ptr<GLProgram> program;

    GLBuffer  cbo;

    Vector<3> lightPos;
};