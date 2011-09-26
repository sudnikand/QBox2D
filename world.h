#ifndef WORLD_H
#define WORLD_H

#include <Box2D.h>
#include <QGraphicsScene>
#include "qbox2ditems.h"

class QBox2DWorld : public b2World {

public:
    QBox2DWorld() : b2World(b2Vec2(0, 0)) { }

    QBox2DWorld(const b2Vec2& gravity) : b2World(gravity) { }

    ~QBox2DWorld() { }

    void step(){
        this->Step(_timeStep,_velocityIterations,_positionIterations);
    }

    void setSettings(float32 timeStep, int32 velIters, int32 posIters){
        _timeStep = timeStep;
        _velocityIterations = velIters;
        _positionIterations = posIters;
    }

    virtual void create(QGraphicsScene* const scene) = 0;

private:
    float32             _timeStep;
    int32               _velocityIterations;
    int32               _positionIterations;

};

class ExampleWorld : public QBox2DWorld {
public:
    ExampleWorld() : QBox2DWorld() {}

    void create(QGraphicsScene* const scene){
        SetGravity(b2Vec2(0, -10));

        // ground
        QBox2DRectItem *ground = new QBox2DRectItem();
        QRectF groundrect(0, 0, 400, 5);
        QColor groundColor(64, 64, 64);
        ground->setShape(groundrect);
        ground->setPos(-200, 0);
        ground->setBrush(groundColor);
        ground->create(this);
        scene->addItem(ground);

        // roof
        QBox2DRectItem *roof = new QBox2DRectItem();
        roof->setShape(groundrect);
        roof->setPos(-200, -405);
        roof->setBrush(groundColor);
        roof->create(this);
        scene->addItem(roof);

        //left wall
        QBox2DRectItem *leftWall = new QBox2DRectItem();
        QRectF rect(0, 0, 5, 400);
        leftWall->setShape(rect);
        leftWall->setPos(-205, -400);
        leftWall->setBrush(groundColor);
        leftWall->create(this);
        scene->addItem(leftWall);

        //right wall
        QBox2DRectItem *rightWall = new QBox2DRectItem();
        rightWall->setShape(rect);
        rightWall->setPos(200, -400);
        rightWall->setBrush(groundColor);
        rightWall->create(this);
        scene->addItem(rightWall);

        //add motors
        for (int i = 0; i < 2; ++i) {
            QBox2DRectItem *staticBox = new QBox2DRectItem();
            staticBox->setShape(QRectF(0, 0, 10, 50));
            staticBox->setPos(pow(-1.0f,i) * 60, -50);
            staticBox->setRotation(0);
            staticBox->setBrush(groundColor);
            staticBox->setBodyType(b2_dynamicBody);
            staticBox->setFriction(1.0f);
            staticBox->setDensity(2.0f);
            staticBox->setRestitution(0.1f);
            staticBox->create(this);
            scene->addItem(staticBox);
            b2RevoluteJointDef jd1;
            jd1.Initialize(staticBox->body(), ground->body(), staticBox->body()->GetPosition());
            jd1.motorSpeed = (float32)(pow(-1.0f,i) * 2 * b2_pi);
            jd1.maxMotorTorque = 50000000.0f;
            jd1.enableMotor = true;
            b2Joint* revJoint = CreateJoint(&jd1);
            //joints.append(revJoint);
        }

        //add falling boxes
        for (int i = 0; i < 10; ++i) {
            int size = 6 + qrand() % 10;
            QBox2DRectItem *item = new QBox2DRectItem();
            item->setShape(QRectF(0,0,size,size));
            item->setPos(-80 + qrand() % 40, -75 - qrand() % 150);
            item->setRotation(qrand() % 360);
            item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            item->setBodyType(b2_dynamicBody);
            item->setDensity(1.0f);
            item->setFriction(0.8f);
            item->setRestitution(0.4f);
            item->create(this);
            scene->addItem(item);
        }

        // add triangles
        for (int i = 0; i < 10; ++i) {
            int size = 4 + qrand() % 7;
            QPolygonF polygonForm;
            polygonForm << QPointF(0, -size*2) << QPointF(-size, 0) << QPointF(size, 0);
            QBox2DPolygonItem *testItem = new QBox2DPolygonItem();
            testItem->setShape(polygonForm);
            testItem->setPos(-20 + qrand() % 60, -50 - qrand() % 150);
            testItem->setRotation(qrand() % 360);
            testItem->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            testItem->setFriction(1.0f);
            testItem->setDensity(5.0f);
            testItem->setRestitution(0.5f);
            testItem->setBodyType(b2_dynamicBody);
            testItem->create(this);
            scene->addItem(testItem);
        }

        //Add circles
        for (int i = 0; i < 600; ++i) {
            int size = 3;
            QBox2DCircleItem *item = new QBox2DCircleItem();
            item->setShape(QRectF(-size , -size, size*2, size*2));
            item->setPos(0 + qrand() % 60, -100 - qrand() % 150);
            item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            item->setBodyType(b2_dynamicBody);
            item->setFriction(0.1f);
            item->setDensity(1.0f);
            item->setRestitution(0.1f);
            item->create(this);
            scene->addItem(item);
        }
    }
};

#endif // WORLD_H
