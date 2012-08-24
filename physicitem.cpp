#include "physicitem.h"

PhysicItem::PhysicItem() :
    _body(NULL),
    _position(b2Vec2(0,0)),
    _rotation(0)
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
}

void PhysicItem::createFixture(){
    if (!_body) return;
    _body->CreateFixture(&_fd);
}

void PhysicItem::setUserData(void *data){
    if (!_body) return;
    _body->SetUserData(data);
}

