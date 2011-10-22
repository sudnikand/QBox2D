#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QGraphicsScene>
#include <QObject>
#include "qbox2ditems.h"

struct ContactPoint {
    b2Fixture* fixtureA;
    b2Fixture* fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
};

class QBox2DWorld : public QObject, public b2ContactListener {
Q_OBJECT

public:
    explicit QBox2DWorld(QObject* parent = 0);

    ~QBox2DWorld() {
        delete _world;
        _world = NULL;
    }

    void setSettings(float32 timeStep, int32 velIters, int32 posIters){
        _timeStep = timeStep;
        _velocityIterations = velIters;
        _positionIterations = posIters;
        _world->SetWarmStarting(true);
        _world->SetContinuousPhysics(true);
    }

    virtual void create(QGraphicsScene* const scene) = 0;

    virtual void updateKeys(int key, int state) {
        Q_UNUSED(key); Q_UNUSED(state);
    }

 public slots:
    void step();
    QBox2DRectItem* createBox(const QPointF &pos);
    void grabItem(const QPointF &p);
    void dropItem();
    void moveItem(const QPointF &p);

public:
    b2World*        _world;
    QGraphicsScene* _scene;

private:
    float32                 _timeStep;
    int32                   _velocityIterations;
    int32                   _positionIterations;
    QList<ContactPoint *>   _contacts;
    uint                    _contactsCount;
    b2MouseJoint*           _mouseJoint;
    b2Body*                 _groundBody;
};

class TestWorld : public QBox2DWorld {
public:
    TestWorld() : QBox2DWorld() {}
    void create(QGraphicsScene* const scene);
private:
    QBox2DRectItem* _box;
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


class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point)
    {
        _point = point;
        _fixture = NULL;
    }

    bool ReportFixture(b2Fixture* fixture)
    {
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody)
        {
            bool inside = fixture->TestPoint(_point);
            if (inside)
            {
                _fixture = fixture;

                // We are done, terminate the query.
                return false;
            }
        }

        // Continue the query.
        return true;
    }

    b2Vec2 _point;
    b2Fixture* _fixture;
};



#endif // WORLD_H
