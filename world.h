#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QGraphicsScene>
#include "qbox2ditems.h"

class QBox2DWorld : public b2ContactListener {

public:
    QBox2DWorld() {
        _world = new b2World(b2Vec2(0,0));
        _world->SetContactListener(this);
        _contactsCount = 0;
    }

    ~QBox2DWorld() {
        delete _world;
        _world = NULL;
    }

    void step(){
        _world->Step(_timeStep,_velocityIterations,_positionIterations);
    }

    void setSettings(float32 timeStep, int32 velIters, int32 posIters){
        _timeStep = timeStep;
        _velocityIterations = velIters;
        _positionIterations = posIters;
        _world->SetWarmStarting(true);
        _world->SetContinuousPhysics(true);
    }

    virtual void create(QGraphicsScene* const scene) = 0;
    virtual void updateKeys(int key, int state) = 0;

public:
    b2World*    _world;

private:
    float32     _timeStep;
    int32       _velocityIterations;
    int32       _positionIterations;
    QList<ContactPoint *> _contacts;
    uint         _contactsCount;

};

#endif // WORLD_H
