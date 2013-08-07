#version 120

attribute vec4 vertex;
uniform mat4 mvpMatrix;


attribute vec2 textureCoordinate;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    gl_Position = mvpMatrix * vertex;
    varyingTextureCoordinate = textureCoordinate;
}
