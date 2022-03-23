#include "Sphere.h"
#include <cmath>

#define RATIO (4.f / sqrtf(6.f))
#define LEVELS (4)
#define RADIUS (0.5f)

using namespace std;

Sphere::Sphere()
{
    vector<Vertex> vertices;
    vector<Coordinate> coords;

    Vertex v0 = { RADIUS, 0, 0 };
    Vertex v1 = { 0, 0, RADIUS };
    Vertex v2 = { 0, RADIUS, 0 };

    // Split 8 faces of an octahedron
    Split(1,  v0,  v1, v2, vertices, coords);
    Split(1,  v0, -v1, v2, vertices, coords);
    Split(1, -v0,  v1, v2, vertices, coords);
    Split(1, -v0, -v1, v2, vertices, coords);

    Split(1,  v0,  v1, -v2, vertices, coords);
    Split(1,  v0, -v1, -v2, vertices, coords);
    Split(1, -v0,  v1, -v2, vertices, coords);
    Split(1, -v0, -v1, -v2, vertices, coords);

    this->Vertices(vertices.data(), vertices.size());

    auto ratio = 1.f / RADIUS;
    for (auto& v : vertices)
    {
        v *= ratio;
    }
    this->Normals(vertices.data(), vertices.size());

    this->TexCoords(coords.data(), (int)coords.size());
}

void Sphere::Split(uint32_t level, const Vertex& v0, const Vertex& v1, const Vertex& v2, vector<Vertex>& vertices, vector<Coordinate>& coords)
{
    if (level > LEVELS)
    {
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);

        coords.push_back({ v0[0] + .5f, .5f - v0[1] });
        coords.push_back({ v1[0] + .5f, .5f - v1[1] });
        coords.push_back({ v2[0] + .5f, .5f - v2[1] });

        return;
    }

    Vertex v[] = { (v0 + v1).Normalize() * RADIUS,
                   (v1 + v2).Normalize() * RADIUS,
                   (v2 + v0).Normalize() * RADIUS };

    Split(level + 1, v0, v[0], v[2], vertices, coords);
    Split(level + 1, v1, v[1], v[0], vertices, coords);
    Split(level + 1, v2, v[2], v[1], vertices, coords);

    Split(level + 1, v[0], v[1], v[2], vertices, coords);
}