#ifndef ITEMS_H
#define ITEMS_H

#include "def.h"
#include "physicitem.h"
#include <QAbstractGraphicsShapeItem>
#include <QBrush>

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
    void setShape(const b2Shape &s);
    QAbstractGraphicsShapeItem* graphics() { return _graphics; }

private:
    QAbstractGraphicsShapeItem* _graphics;
};


#endif // ITEMS_H
