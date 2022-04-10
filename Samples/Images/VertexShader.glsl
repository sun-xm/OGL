#version 400 core

uniform vec2 HalfWH;
uniform mat4 Rotate;

in vec3 vtx;
in vec2 crd;

out vec2 coord;

void main()
{
    vec4 pos = Rotate * vec4(vtx, 1);
    gl_Position = vec4(pos.x / HalfWH.x, pos.y / HalfWH.y, 0, 1);
    coord = crd;
}