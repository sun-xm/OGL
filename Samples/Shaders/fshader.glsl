#version 400 core

uniform float Ambient;

in vec3  color;
in float lumen;

void main()
{
    gl_FragColor = mix(vec4(color * lumen, 1), vec4(color, 1), Ambient);
}