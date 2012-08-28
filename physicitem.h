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
            void setBodyType   (const b2BodyType & bt) { _bodytype = bt; }
            void setRotation   (const float32 & r)     { _rotation = r; }
            void setPos        (const b2Vec2 & p)      { _position = p; }
            void setShape      (const b2Shape &s )     { _fd.shape = &s; }
            void setUserData   (void *data);
            void createFixture ();
            void createBody    (b2World *const world);

            b2Body*    body()     const { return _body; }
            b2BodyType bodyType() const { return _bodytype; }
            b2Vec2     position() const { return _position; }
            float32    rotation() const { return _rotation; }



protected:
            b2Body*      _body;
            b2BodyType   _bodytype;
            b2FixtureDef _fd;
            b2Vec2       _position;
            float32      _rotation;
};

#endif // PHYSICITEM_H
