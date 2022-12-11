#version 400 core

uniform vec3 LightPos;
uniform mat4 WorldView;
uniform mat4 ModelView;
uniform mat4 Projection;

in vec3 vtx;
in vec3 clr;
in vec3 nml;

out vec3  color;
out float lumen;

void main()
{
    vec4 vpos = WorldView * ModelView * vec4(vtx, 1);
    gl_Position = Projection * vpos;

    vec4 lpos = WorldView * vec4(LightPos, 1);
    vec3 nvec = mat3(WorldView * ModelView) * nml;

    lumen = max(0, dot(nvec, normalize(lpos.xyz - vpos.xyz)));
    lumen = dot(nvec, normalize(vpos.xyz)) > 0 ? 0 : lumen;
    color = clr;
}