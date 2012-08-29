#ifndef GLITEM_H
#define GLITEM_H

#include "physicitem.h"
#include <QtOpenGL>

class GLItem
{
public:
    GLItem();
    void render();
    void createBody    (b2World* const world);
    void setBrush      (const QColor  & c  )     { _color = c; }
    void setPos        (const float32 &x, const float32 &y) { _physic.setPos(b2Vec2(x,y)); }
    void setPos        (const b2Vec2  & pos)     { _physic.setPos(pos); }
    void setRotation   (const float32 & rot)     { _physic.setRotation(rot); }
    void setShape      (const b2Shape & s  )     { _physic.setShape(s); }
    void setRestitution(const float32 & r  )     { _physic.setRestitution(r); }
    void setDensity    (const float32 & d  )     { _physic.setDensity(d); }
    void setFriction   (const float32 & f  )     { _physic.setFriction(f); }
    void setBodyType   (const b2BodyType & bt)   { _physic.setBodyType(bt); }

    b2Body* body() const { return _physic.body(); }

private:
    PhysicItem _physic;
    QColor     _color;
};

#endif // GLITEM_H
