#version 400 core

uniform vec3 LightPos;
uniform mat4 WorldView;
uniform mat4 ModelView;
uniform mat4 Projection;

varying vec3 color;

attribute vec3 vtx;
attribute vec3 clr;
attribute vec3 nml;

void main()
{
    vec4 vpos = ModelView * vec4(vtx, 1);
    gl_Position = Projection * vpos;

    vec4 lpos = WorldView * vec4(LightPos, 1);
    vec3 nvec = mat3(ModelView) * nml;

    float lumen = max(0, dot(nvec, normalize(lpos.xyz - vpos.xyz)));
    lumen = dot(nvec, normalize(vpos.xyz)) > 0 ? 0 : lumen;

    color = clr * lumen;
}