#version 400 core

in vec3 vtx;
in vec2 crd;

out vec2 coord;

void main()
{
    gl_Position = vec4(vtx, 1);
    coord = crd;
}