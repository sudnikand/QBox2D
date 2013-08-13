#include "texture.h"

#include <QtOpenGL>
#include <QtGui/QImage>

Texture::Texture(const QString &fileName, bool clamp) :
    _refCount(1),
    _textureId(0)
{
    if( !fileName.isEmpty() ) {
        load( fileName, clamp );
    }
}

Texture::~Texture()
{
    if(_textureId)
        glDeleteTextures(1, &_textureId);
}

bool Texture::load(const QString &fileName, bool clamp)
{
    qDebug() << "Loading: [Texture] " << fileName;

    if(fileName.isEmpty())
		return false;

    QImage image, imageGL;

    if( !image.load(fileName) )
		return false;

    imageGL = QGLWidget::convertToGLFormat(image);

    _width  = imageGL.width();
    _height = imageGL.height();

    glGenTextures( 1, &_textureId );

	enable();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if( clamp )	{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, imageGL.constBits() );

	disable();

	return true;
}

Texture *Texture::grab(){
    _refCount++;
	return this;
}

bool Texture::drop()
{
    if( --_refCount == 0 ){
		delete this;
		return true;
	}
	return false;
}
