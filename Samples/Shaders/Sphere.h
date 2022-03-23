#pragma once

#include "GLShape.h"
#include "Program.h"
#include <vector>

class Sphere : public GLShape
{
public:
    Sphere(Program& program);

    void Create();
    void Release() override;

    bool Colors(const Vector<float, 3>* colors, int count);
    void Render(const GLScene& scene, const Vertex& lightPos);

protected:
    size_t Apply(const GLScene& scene) override;
    void   Revoke() override;

private:
    static void Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices);

    Program& program;
    GLBuffer cbo;

    Vector<float, 3> lightPos;
};