#include "glitem.h"
#include <GL/gl.h>

GLItem::GLItem()
{
}

void GLItem::createBody(b2World* const world){
    _physic.createBody(world);
    _physic.setUserData(this);
}

void GLItem::render(){
    if(!_physic.body()) return;
    glPushMatrix();

    const b2Vec2 &position = body()->GetPosition();
    glTranslatef(position.x,position.y,0.0f);
    glRotatef(body()->GetAngle(),0.0f,0.0f,1.0f);

    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glColor3f(0,1,0);
    glVertex3f(50,0,0);
    glColor3f(0,0,1);
    glVertex3f(50,50,0);
    glEnd();

    glPopMatrix();
}
