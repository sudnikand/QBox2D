#ifndef QBOX2DGRAPHICS_H
#define QBOX2DGRAPHICS_H

#include <QGraphicsItem>
#include <QPainter>
#include <Box2D.h>
#include "def.h"

class QBox2DItem : public QGraphicsItem {
public:

    QBox2DItem(QGraphicsItem* parent = 0) : QGraphicsItem(parent) { }
    virtual ~QBox2DItem(){}

    void setRestitution(const float32 & r)     { _fd.restitution = r; }
    void setDensity    (const float32 & d)     { _fd.density = d; }
    void setFriction   (const float32 & f)     { _fd.friction = f; }
    void setBodyType   (const b2BodyType & bt) { _bodytype = bt; }
    void setBrush      (const QBrush & brush)  { _brush = brush; update(); }
    void setPen        (const QPen & pen)      { _pen = pen; update(); }

    b2Body* body() const { return _body; }

protected:

    void createBody(b2World* const world){
        b2BodyDef bd;
        bd.position.Set(Q2W(x(),-y()));
        bd.type = _bodytype;
        bd.angle = ANG2RAD(-rotation());
        _body = world->CreateBody(&bd);
    }

    void advance(int step) {
        if (!step) return;
        if (!_body->IsAwake()) return;
        if (_bodytype == b2_staticBody) return;
        const b2Vec2 &position = _body->GetPosition();
        setPos(W2Q(position.x, -position.y));
        setRotation(RAD2ANG(-_body->GetAngle()));
    }

protected:
            b2Body*      _body;
            b2BodyType   _bodytype;
            b2FixtureDef _fd;
    mutable QRectF       _boundingRect;
            QBrush       _brush;
            QPen         _pen;
};


class QBox2DRectItem : public QBox2DItem {
public:

    QBox2DRectItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    void create(b2World* const world);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    QRectF boundingRect() const;
    void setShape(const QRectF& rect);

private:
    QRectF _rect;
};



class QBox2DPolygonItem : public QBox2DItem {
public:

    QBox2DPolygonItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    void   create(b2World* const world);
    void   paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void   setShape(const QPolygonF& polygon);
    QRectF boundingRect() const;

private:
    QPolygonF _polygon;
};



class QBox2DCircleItem : public QBox2DItem {
public:

    QBox2DCircleItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    void create(b2World* const world);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setShape(const QRectF& rect);
    QRectF boundingRect() const;

private:
    QRectF _rect;
};

#endif // QBOX2DGRAPHICS_H
