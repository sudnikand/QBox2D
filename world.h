#ifndef WORLD_H
#define WORLD_H

#include "items.h"
#include <Box2D.h>
#include <QObject>
#include <QSet>

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

protected:
    QList<QBox2DItem*>      _items;
    QList<ContactPoint>     _contacts;
    b2Body*                 _ground;

private:
    float32                 _timeStep;
    int32                   _velocityIterations;
    int32                   _positionIterations;
    b2MouseJoint*           _mouseJoint;

public:
    explicit QBox2DWorld(QObject* parent = 0);
    virtual ~QBox2DWorld();

            void setSettings(float32 timeStep, int32 velIters, int32 posIters);
    virtual void populate() = 0;
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
            void destroyItem(QBox2DItem *item);
            void appendItem(QBox2DItem* item);
            void removeItem(QBox2DItem *item);
            void loadWorld(const QString &filename);
     QBox2DItem* findItem(const QString &itemName);

public slots:
    virtual void step();
    virtual void handleContact(const ContactPoint &cp);
    virtual void handleKeyPressed(const int &key);
    virtual void handleKeyReleased(const int &key);
            void grabItem(const QPointF &p);
            void dropItem();
            void moveItem(const QPointF &p);
            QBox2DItem* createBox(const QPointF &pos);

signals:
            void contact(const ContactPoint &cp);
            void itemCreated(QBox2DItem *item);
            void itemDestroyed(QBox2DItem *item);
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
