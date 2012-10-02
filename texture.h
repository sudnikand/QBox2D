#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include <QtOpenGL>

class Texture {
public:
    Texture(const QString &fileName = "", bool clamp = true );
	virtual ~Texture();

    bool load( const QString &fileName, bool clamp = true );

    void enable()  { glBindTexture(GL_TEXTURE_2D, _textureId); }
    void disable() { glBindTexture(GL_TEXTURE_2D, 0); }

    Texture* grab();
    bool     drop();

    int width()  const { return _width; }
    int height() const { return _height; }

    GLuint textureId() const { return _textureId; }

protected:
    int refCount() const { return _refCount; }

protected:
    int    _refCount;
    int    _width;
    int    _height;
    GLuint _textureId;
};

#endif // TEXTURE_H
