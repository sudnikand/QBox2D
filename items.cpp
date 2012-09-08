#include "items.h"

void QBox2DItem::setShape(const b2Shape &s) {
    if (!_body) return;

    PhysicItem::setShape(s);
    b2Shape::Type shapeType = s.GetType();
    QAbstractGraphicsShapeItem *graphics;

    if (shapeType == b2Shape::e_polygon){

        const b2PolygonShape shape = static_cast<const b2PolygonShape&>(s);
        QPolygonF polygon;

        for (int32 i = 0; i< shape.GetVertexCount(); ++i){
            b2Vec2 v = shape.GetVertex(i);
            polygon.append(QPointF(W2Q(v.x,v.y)));
        }

        graphics = new QGraphicsPolygonItem(polygon);
    }
    else if (shapeType == b2Shape::e_circle){

        const b2CircleShape shape = static_cast<const b2CircleShape&>(s);
        float32 radius = shape.m_radius;
        QRectF rect(-W2Q_(radius),-W2Q_(radius),W2Q_(radius*2), W2Q_(radius*2));

        graphics = new QGraphicsEllipseItem(rect);
    }

    setGraphics(graphics);
    graphics->setPos(W2Q(position().x,position().y));
    graphics->setRotation(RAD2ANG(rotation()));

}
