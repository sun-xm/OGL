#version 400 core

uniform mat3 Matrix;
uniform vec2 Unify;

in vec3 vtx;
in vec2 crd;

out vec2 coord;

void main()
{
    vec3 pos = Matrix * vtx;
    gl_Position = vec4(pos.x / Unify.x, pos.y / Unify.y, 0, 1);
    coord = crd;
}