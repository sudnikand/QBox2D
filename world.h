#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QGraphicsScene>
#include "qbox2ditems.h"

struct ContactPoint {
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
};

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
    virtual void updateKeys(int key, int state) {}

    QBox2DRectItem* createBox(QPointF pos) {
        QBox2DRectItem *box = new QBox2DRectItem();
        box->setBodyType(b2_dynamicBody);
        QRectF rect(0, 0, 10, 10);
        box->setShape(rect);
        box->setPos(pos.x()-10.0, pos.y()-10.0);
        box->setFriction(0.9f);
        box->setDensity(1.0f);
        box->setRestitution(0.5f);
        box->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        box->create(_world);
        return box;
    }

public:
    b2World*    _world;

private:
    float32     _timeStep;
    int32       _velocityIterations;
    int32       _positionIterations;
    QList<ContactPoint *> _contacts;
    uint         _contactsCount;

};

class ExampleWorld : public QBox2DWorld {
public:
    ExampleWorld() : QBox2DWorld() {}
    void create(QGraphicsScene* const scene);
};

class ArcanoidWorld : public QBox2DWorld {
public:
    ArcanoidWorld() : QBox2DWorld() {}
    void create(QGraphicsScene* const scene);
    void updateKeys(int key, int state);
private:
    QBox2DRectItem* _paddle;
};


#endif // WORLD_H
