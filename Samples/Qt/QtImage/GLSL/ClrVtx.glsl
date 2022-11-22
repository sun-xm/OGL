#version 330 core

uniform mat3 Matrix;

in vec3 vtx;

void main()
{
    gl_Position = vec4((Matrix * vtx).xy, 0, 1);
}