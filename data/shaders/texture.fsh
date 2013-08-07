uniform vec4 color;
uniform sampler2D texture;
varying vec2 varyingTextureCoordinate;

void main(void)
{
    gl_FragColor = texture2D(texture, varyingTextureCoordinate);
}
