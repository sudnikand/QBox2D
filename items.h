#ifndef ITEMS_H
#define ITEMS_H

#include "def.h"
#include "physicitem.h"
#include <QGraphicsItem>
#include <QPainter>

class QBox2DItem : public QGraphicsItem {
public:

    QBox2DItem(QGraphicsItem* parent = 0) : QGraphicsItem(parent) { }
    virtual ~QBox2DItem(){}

    void setRestitution(const float32 & r)     { _physic.setRestitution(r); }
    void setDensity    (const float32 & d)     { _physic.setDensity(d); }
    void setFriction   (const float32 & f)     { _physic.setFriction(f); }
    void setBodyType   (const b2BodyType & bt) { _physic.setBodyType(bt); }
    void setShapeB2    (const b2Shape & s )    { _physic.setShape(s); }
    void setBrush      (const QBrush & brush)  { _brush = brush; update(); }
    void setPen        (const QPen & pen)      { _pen = pen; update(); }
    void createFixture ()                      { _physic.createFixture(); }

    b2Body* body() const { return _physic.body(); }
    b2BodyType bodyType() const { return _physic.bodyType(); }

    void setPos(const QPointF &pos){
        _physic.setPos(b2Vec2(Q2W(pos.x(),-pos.y())));
        QGraphicsItem::setPos(pos);
    }

    void setPos(const qreal &x, const qreal &y){
        _physic.setPos(b2Vec2(Q2W_(x),-Q2W_(y)));
        QGraphicsItem::setPos(x,y);
    }


protected:

    void createBody(b2World* const world){
        _physic.createBody(world);
        _physic.setUserData(this);
    }

    void advance(int step) {
        if (!step) return;
        if (!body()->IsAwake()) return;
        if (bodyType() == b2_staticBody) return;
        const b2Vec2 &position = body()->GetPosition();
        QGraphicsItem::setPos(W2Q(position.x, -position.y));
        setRotation(RAD2ANG(-body()->GetAngle()));
    }

protected:
    mutable QRectF       _boundingRect;
            QBrush       _brush;
            QPen         _pen;
            PhysicItem   _physic;
};


class QBox2DRectItem : public QBox2DItem {
public:

    QBox2DRectItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    QBox2DRectItem(const QRectF &rect, QGraphicsItem* parent = 0) :
        QBox2DItem(parent) {
        _rect = rect;
    }

    void create(b2World* const world);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    QRectF boundingRect() const;
    void setShape(const QRectF &rect);

private:
    QRectF _rect;
};



class QBox2DPolygonItem : public QBox2DItem {
public:

    QBox2DPolygonItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    void   create(b2World* const world);
    void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void   setShape(const QPolygonF &polygon);
    QRectF boundingRect() const;

private:
    QPolygonF _polygon;
};



class QBox2DCircleItem : public QBox2DItem {
public:

    QBox2DCircleItem(QGraphicsItem* parent = 0) : QBox2DItem(parent) { }

    void create(b2World* const world);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    void setShape(const QRectF &rect);
    QRectF boundingRect() const;

private:
    QRectF _rect;
};

#endif // ITEMS_H
