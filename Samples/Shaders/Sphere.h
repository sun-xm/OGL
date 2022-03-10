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
    void Split(float radius, int recursion, const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Vertex>& vertices, std::vector<Normal>& normals);

    int recursion;

    Program& program;
    GLBuffer cbo;

    Vector<float, 3> lightPos;

    static const Vertex RegularTetrahedron[4];
};