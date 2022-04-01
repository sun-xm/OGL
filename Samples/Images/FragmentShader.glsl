#version 400 core

uniform sampler2D tex;

in vec2 coord;

void main()
{
    gl_FragColor = texture(tex, coord);
}