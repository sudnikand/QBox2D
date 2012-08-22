#include "physicitem.h"

PhysicItem::PhysicItem() : _position(b2Vec2(0,0)), _rotation(0)
{
}

PhysicItem::~PhysicItem()
{
}

void PhysicItem::createBody(b2World *const world){
    b2BodyDef bd;
    bd.position = _position;
    bd.type = _bodytype;
    bd.angle = _rotation;
    _body = world->CreateBody(&bd);
    _body->CreateFixture(&_fd);
}
