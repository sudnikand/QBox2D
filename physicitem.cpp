#include "physicitem.h"

PhysicItem::PhysicItem() :
    _body(NULL)
{
}

PhysicItem::~PhysicItem()
{
}

void PhysicItem::createBody(b2World *const world){
    if(_body) {
        _bd.position = position();
        _bd.angle = rotation();
        _bd.type = bodyType();
        world->DestroyBody(_body);
    }
    _body = world->CreateBody(&_bd);
}

void PhysicItem::setShape(const b2Shape &s ){
    if(!_body) return;
    _fd.shape = &s;
    _body->CreateFixture(&_fd);
}

void PhysicItem::setUserData(void *data){
    if (!_body) return;
    _body->SetUserData(data);
}

