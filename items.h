#ifndef ITEMS_H
#define ITEMS_H

#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QtOpenGL>
#include "def.h"
#include "physicitem.h"

class GLScene;

class QBox2DItem : public PhysicItem {
public:

    QBox2DItem() :
        _graphics(NULL),
        _glmode(GL_TRIANGLE_FAN)
    { }

    virtual ~QBox2DItem() { if(_graphics) delete _graphics; }

    virtual int handleContact();

    void setGraphics(QAbstractGraphicsShapeItem* gi);
    void setColor(const QColor &c);
    void setName(const QString &name);
    void update();
    void draw();
    void setVertices(const QVector<QVector3D> &vertices);
    void setTextureName(const QString &textureName);
    void setTextureID(const GLuint &textureID);
    void setGLmode(const GLenum &mode);

    QAbstractGraphicsShapeItem* graphics()    const;
    QVector<QVector3D>          vertices()    const;
    QMatrix4x4&                 modelMatrix();
    const QColor                color()       const;
    const QString               name()        const;
    const QString               textureName() const;
          GLuint                textureID()   const;
          GLenum                GLmode()      const;

    GLScene*           _glscene;

private:
    QAbstractGraphicsShapeItem* _graphics;
    QColor             _color;
    QString            _name;
    QMatrix4x4         _mMatrix;
    QString            _textureName;
    GLuint             _textureID;
    QVector<QVector3D> _vertices;
    GLenum             _glmode;

};


#endif // ITEMS_H
