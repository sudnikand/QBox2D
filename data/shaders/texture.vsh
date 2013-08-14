#version 120

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

attribute vec3 vertex;
attribute vec2 textureCoordinate;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
    varyingTextureCoordinate = textureCoordinate;
}
