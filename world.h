#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QObject>
#include <QSet>
#include <QDomDocument>
#include "items.h"
#include "contactlistener.h"

class QBox2DWorld : public QObject {
Q_OBJECT

public:
    b2World*                _world;
    QBox2DContactListener*  _contactListener;

protected:
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
            void destroyItem(QBox2DItem *item);
            void appendItem(QBox2DItem *item);
            void loadWorld(const QString &filename);
     QBox2DItem* findItem(const QString &itemName);

public slots:
    virtual void step();
    virtual void handleKeyPressed(const int &key);
    virtual void handleKeyReleased(const int &key);
    virtual void populate() = 0;
            void grabItem(const QPointF &p);
            void dropItem();
            void moveItem(const QPointF &p);
            QBox2DItem* createBox(const QPointF &pos);

signals:
            void itemCreated(QBox2DItem *item);
            void itemDestroyed(QBox2DItem *item);
            void gameFinished();

private:
            void parseXML(const QDomElement &root);
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
