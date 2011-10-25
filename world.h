#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QGraphicsScene>
#include <QObject>
#include <QDebug>
#include "items.h"

struct ContactPoint {
    QBox2DItem* itemA;
    QBox2DItem* itemB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
};

class QBox2DWorld : public QObject, public b2ContactListener {
Q_OBJECT

public:
    b2World*                _world;
    QGraphicsScene*         _scene;

protected:
    QList<ContactPoint>     _contacts;
    b2Body*                 _groundBody;

private:
    float32                 _timeStep;
    int32                   _velocityIterations;
    int32                   _positionIterations;
    b2MouseJoint*           _mouseJoint;

public:
    explicit QBox2DWorld(QObject* parent = 0);
    ~QBox2DWorld();

            void setSettings(float32 timeStep, int32 velIters, int32 posIters);
    virtual void create(QGraphicsScene* const scene) = 0;
    virtual void updateKeys(int key, int state);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

public slots:
    virtual void step();
    virtual void handleContact(const ContactPoint &cp);
            void grabItem(const QPointF &p);
            void dropItem();
            void moveItem(const QPointF &p);
            QBox2DRectItem* createBox(const QPointF &pos);

signals:
            void contact(const ContactPoint &cp);
};


class QueryCallback : public b2QueryCallback
{
public:
    QueryCallback(const b2Vec2& point){
        _point = point;
        _fixture = NULL;
    }

    bool ReportFixture(b2Fixture* fixture){
        b2Body* body = fixture->GetBody();
        if (body->GetType() == b2_dynamicBody){
            bool inside = fixture->TestPoint(_point);
            if (inside){
                _fixture = fixture;

                // We are done, terminate the query.
                return false;
            }
        }

        // Continue the query.
        return true;
    }

    b2Vec2     _point;
    b2Fixture* _fixture;
};



#endif // WORLD_H
