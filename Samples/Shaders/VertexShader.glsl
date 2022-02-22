#version 400 core

uniform vec3 LightPos;
uniform mat4 WorldView;
uniform mat4 ModelView;
uniform mat4 Projection;

layout (location = 0) in vec3 vtx;
layout (location = 1) in vec3 clr;
layout (location = 2) in vec3 nml;

out vec4 color;

void main()
{
    vec4 vpos = ModelView * vec4(vtx, 1);
    gl_Position = Projection * vpos;

    vec4 lpos = WorldView * vec4(LightPos, 1);
    vec3 nvec = mat3(ModelView) * nml;

    float lumen = max(0, dot(nvec, normalize(lpos.xyz - vpos.xyz)));

    if (dot(nvec.xyz, normalize(vpos.xyz)) > 0)
    {
        lumen = 0;
    }

    color = vec4(clr * lumen, 1);
}