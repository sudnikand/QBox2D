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
           _vertices.append(QVector3D(0, 0, -2));
           int accuracy = 32;
           float32 radius = shape->m_radius;
           float32 a = 2 * PI/accuracy;
           for(int i = 0; i < accuracy; ++i) {
               _vertices.append(QVector3D(cos(a*i) * radius, sin(a*i) * radius, -2));
           }
           _vertices.append(QVector3D(radius, 0, -2));
       }
    _mMatrix.setToIdentity();
    _mMatrix.translate(position().x,position().y,0);
    _mMatrix.rotate(RAD2ANG(rotation()), QVector3D(0,0,1));

    if(!graphics()) return;
    graphics()->setPos(W2Q(position().x,position().y));
    graphics()->setRotation(RAD2ANG(rotation()));
}
