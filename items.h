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

    void setGraphics(QAbstractGraphicsShapeItem* gi) {
        if(_graphics) {
            delete _graphics;
        }
        _graphics = gi;
    }

    void setColor(const QColor &c) {
        _color = c;
        if (graphics()) {
            graphics()->setBrush(c);
        }
    }

    void setName(const QString &name){
        _name = name;
    }

    void update();

    QAbstractGraphicsShapeItem* graphics() const { return _graphics; }
    const QColor color() const { return _color; }
    const QString name() const { return _name; }
    QVector<QVector3D> _vertices;
    QMatrix4x4         _mMatrix;

private:
    QAbstractGraphicsShapeItem* _graphics;
    QColor  _color;
    QString _name;


};


#endif // ITEMS_H
