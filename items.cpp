#include "items.h"

void QBox2DItem::update(){
    if(!body()) return;
    const b2Shape *shape = body()->GetFixtureList()->GetShape();
    if (!shape) return;
    _vertices.clear();
    if( shape->GetType() == b2Shape::e_polygon ) {
           for (int32 i = 0; i < ((b2PolygonShape*)shape)->GetVertexCount(); ++i){
               const b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(i);
               _vertices.append(QVector3D(vertex.x, vertex.y, -2));
           }
       } else if (shape->GetType() == b2Shape::e_circle ) {
        float32 radius = shape->m_radius;
        /*
           _vertices.append(QVector3D(0, 0, -2));
           int accuracy = 32;

           float32 a = 2 * PI/accuracy;
           for(int i = 0; i < accuracy; ++i) {
               _vertices.append(QVector3D(cos(a*i) * radius, sin(a*i) * radius, -2));
           }
           _vertices.append(QVector3D(radius, 0, -2));
           */
        _vertices.append(QVector3D( -radius, -radius, -2 ));
        _vertices.append(QVector3D( -radius,  radius, -2 ));
        _vertices.append(QVector3D(  radius,  radius, -2 ));
        _vertices.append(QVector3D(  radius, -radius, -2 ));
       }

    _mMatrix.setToIdentity();
    _mMatrix.translate(position().x,position().y,0);
    _mMatrix.rotate(RAD2ANG(rotation()), QVector3D(0,0,1));

    if(!graphics()) return;
    graphics()->setPos(position().x,position().y);
    graphics()->setRotation(RAD2ANG(rotation()));
}

QAbstractGraphicsShapeItem* QBox2DItem::graphics() const { return _graphics; }
QVector<QVector3D>          QBox2DItem::vertices() const { return _vertices; }
const QColor QBox2DItem::color() const { return _color; }
const QString QBox2DItem::name() const { return _name; }
const QString QBox2DItem::textureName() const { return _textureName; }
const QMatrix4x4 QBox2DItem::modelMatrix() const { return _mMatrix; }


void QBox2DItem::setGraphics(QAbstractGraphicsShapeItem* gi) {
    if(_graphics) {
        delete _graphics;
    }
    _graphics = gi;
}

void QBox2DItem::setColor(const QColor &c) {
    _color = c;
    if (graphics()) {
        graphics()->setBrush(c);
    }
}

void QBox2DItem::setName(const QString &name){
    _name = name;
}

void QBox2DItem::setTextureName(const QString &textureName){
    _textureName = textureName;
}

int QBox2DItem::handleContact() {
    int durability = bodyType() == b2_dynamicBody ? -1 : 0;
    return durability;
}
