#include "items.h"


void QBox2DItem::render(){
    if(!body()) return;

    const b2Shape *shape = body()->GetFixtureList()->GetShape();
    if (!shape) return;
    glPushMatrix();

    glTranslatef(W2Q_(position().x), W2Q_(position().y), 0.0f);
    glRotatef(RAD2ANG(rotation()),0,0,1.0f);

    QVector<QVector2D> vertices;
    if( shape->GetType() == b2Shape::e_polygon ) {
        glColor3f(_color.redF(), _color.greenF(), _color.blueF());
        for (int32 i=0; i < ((b2PolygonShape*)shape)->GetVertexCount(); ++i){
            const b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(i);
            vertices.append(QVector2D(W2Q(vertex.x,vertex.y)));
        }
    } else if (shape->GetType() == b2Shape::e_circle ) {
        glColor3f(_color.redF(), _color.greenF(), _color.blueF());
        vertices.append(QVector2D(0,0));
        int accuracy = 32;
        float32 radius = W2Q_(shape->m_radius);
        float32 a = 2 * PI/accuracy;
        for(int i = 0; i < accuracy; ++i) {
            vertices.append(QVector2D(cos(a*i) * radius, sin(a*i) * radius));
        }
        vertices.append(QVector2D(radius, 0));
    }

    glVertexPointer(2, GL_FLOAT, 0, vertices.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());

    glPopMatrix();
}

void QBox2DItem::update(){
    if(!graphics()) return;

    graphics()->setPos(W2Q(position().x,position().y));
    graphics()->setRotation(RAD2ANG(rotation()));
}
