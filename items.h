#ifndef ITEMS_H
#define ITEMS_H

#include "def.h"
#include "physicitem.h"
#include <QAbstractGraphicsShapeItem>
#include <QBrush>
#include <QtOpenGL>

class QBox2DItem : public PhysicItem {
public:

    QBox2DItem() :
        _graphics(NULL)
    { }

    virtual ~QBox2DItem() { if(_graphics) delete _graphics; }

    virtual int handleContact();

    void setGraphics(QAbstractGraphicsShapeItem* gi);
    void setColor(const QColor &c);
    void setName(const QString &name);
    void setTextureName(const QString &textureName);
    void update();
    void setVertices(const QVector<QVector3D> &vertices);

    QAbstractGraphicsShapeItem* graphics()    const;
    QVector<QVector3D>          vertices()    const;
    QMatrix4x4&                 modelMatrix();
    const QColor                color()       const;
    const QString               name()        const;
    const QString               textureName() const;

private:
    QAbstractGraphicsShapeItem* _graphics;
    QColor             _color;
    QString            _name;
    QMatrix4x4         _mMatrix;
    QString            _textureName;
    QVector<QVector3D> _vertices;
};


#endif // ITEMS_H
