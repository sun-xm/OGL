#include "Cone.h"

using namespace std;

const int   Pieces = 36;
const float Radius = .5f;
const float Height = 1.f;

Cone::Cone()
{
    vector<Vertex> vertices;
    vertices.reserve(Pieces * 3);

    vector<Normal> normals;
    normals.reserve(vertices.capacity());

    vector<Coordinate> coords;
    coords.reserve(vertices.capacity());

    Vertex tip = { 0.f, 0.f, Height };

    Vertex v;
    v.X = Radius * cosf(0);
    v.Y = Radius * sinf(0);
    v.Z = 0;

    Coordinate c;
    c.X = v.X + .5f;
    c.Y = .5f - v.Y;

    auto q = Quaternion<>::From2Vectors(tip, tip - v);
    Normal n = q.Rotate(v).Normalize();

    float delta = 3.14159265358979323846f / Pieces;
    for (int i = 0; i < Pieces * 2;)
    {
        vertices.push_back(v);
        normals.push_back(n);
        coords.push_back(c);

        i++;
        v.X = Radius * cosf(i * delta);
        v.Y = Radius * sinf(i * delta);

        q = Quaternion<>::From2Vectors(tip, tip - v);
        n = q.Rotate(v).Normalize();

        vertices.push_back(tip);
        normals.push_back(n);
        coords.push_back({ .5f, .5f });

        i++;

        v.X = Radius * cosf(i * delta);
        v.Y = Radius * sinf(i * delta);
        c.X = v.X + .5f;
        c.Y = .5f - v.Y;

        q = Quaternion<>::From2Vectors(tip, tip - v);
        n = q.Rotate(v).Normalize();

        vertices.push_back(v);
        normals.push_back(n);
        coords.push_back(c);
    }

    this->Vertices(vertices.data(), vertices.size());
    this->Normals(normals.data(), normals.size());
    this->TexCoords(coords.data(), coords.size());
}