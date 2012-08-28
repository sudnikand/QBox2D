#include "glitem.h"
#include "def.h"
GLItem::GLItem()
{
}

void GLItem::createBody(b2World* const world){
    _physic.createBody(world);
    _physic.setUserData(this);
}

void GLItem::render(){
    if(!_physic.body()) return;
    
    const b2Shape *shape = body()->GetFixtureList()->GetShape();
    if (!shape) return;
    glPushMatrix();

    const b2Vec2 &position = body()->GetPosition();
    glTranslatef(position.x, position.y, 0.0f);
    const float32 &angle = body()->GetAngle();
    glRotatef(RAD2ANG(angle),0,0,1.0f);
    glBegin(GL_TRIANGLE_FAN);
    
    if( shape->GetType() == b2Shape::e_polygon ) {
        glColor3f(_color.redF(), _color.greenF(), _color.blueF());
        for (int32 i=0; i < ((b2PolygonShape*)shape)->GetVertexCount(); ++i){
            const b2Vec2 vertex = ((b2PolygonShape*)shape)->GetVertex(i);
            glVertex2f(vertex.x, vertex.y);
        }
    } else if (shape->GetType() == b2Shape::e_circle ) {
        glColor3f(_color.redF(), _color.greenF(), _color.blueF());
        glVertex2f(0,0);
        int accuracy = 32;
        float32 radius = shape->m_radius;
        float32 a = 2 * PI/accuracy;
        for(int i = 0; i < accuracy; ++i) {
            glVertex2f( cos(a*i) * radius, sin(a*i) * radius);
        }
        glVertex2f(radius, 0);
    }


    glEnd();

    glPopMatrix();
}
