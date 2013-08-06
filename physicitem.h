#ifndef PHYSICITEM_H
#define PHYSICITEM_H
#include "def.h"
#include <Box2D.h>

class PhysicItem
{
public:
            PhysicItem();
            virtual ~PhysicItem();
            void setRestitution(const float32 & r)     { _fd.restitution = r; }
            void setDensity    (const float32 & d)     { _fd.density = d; }
            void setFriction   (const float32 & f)     { _fd.friction = f; }
            void setBodyType   (const b2BodyType & bt) { _bd.type = bt; }
            void setRotation   (const float32 & r)     { _bd.angle = ANG2RAD(r); }
            void setPos        (const b2Vec2 & p)      { _bd.position = p; }
            void setShape      (const b2Shape &s );
            void setUserData   (void *data);
            void createBody    (b2World *const world);

            b2Body*    body()     const { return _body; }
            b2BodyType bodyType() const { return _body->GetType(); }
            b2Vec2     position() const { return _body->GetPosition(); }
            float32    rotation() const { return _body->GetAngle(); }



private:
            b2Body*      _body;
            b2FixtureDef _fd;
            b2BodyDef    _bd;
};

#endif // PHYSICITEM_H
