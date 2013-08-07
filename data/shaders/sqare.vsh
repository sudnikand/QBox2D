#version 120

uniform mat4 mvpMatrix;
attribute vec4 vertex;

void main(void)
{
    gl_Position = mvpMatrix * vertex;
}

