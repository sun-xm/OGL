#version 400 core

uniform mat3 Matrix;

in vec3 vtx;
in vec2 crd;

out vec2 uv;

void main()
{
    gl_Position = vec4((Matrix * vtx).xy, 0, 1);
    uv = crd;
}