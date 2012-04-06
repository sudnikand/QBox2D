#include "exampleworld.h"

ExampleWorld::ExampleWorld() : QBox2DWorld(){
}

void ExampleWorld::create(){
    _world->SetGravity(b2Vec2(0, -10));

    // ground
    QBox2DRectItem *ground = new QBox2DRectItem();
    QRectF groundrect(0, 0, 400, 5);
    QColor groundColor(64, 64, 64);
    ground->setShape(groundrect);
    ground->setPos(-200, 0);
    ground->setBrush(groundColor);
    ground->create(_world);
    emit itemCreated(ground);

    // roof
    QBox2DRectItem *roof = new QBox2DRectItem();
    roof->setShape(groundrect);
    roof->setPos(-200, -405);
    roof->setBrush(groundColor);
    roof->create(_world);
    emit itemCreated(roof);

    //left wall
    QBox2DRectItem *leftWall = new QBox2DRectItem();
    QRectF rect(0, 0, 5, 400);
    leftWall->setShape(rect);
    leftWall->setPos(-205, -400);
    leftWall->setBrush(groundColor);
    leftWall->create(_world);
    emit itemCreated(leftWall);

    //right wall
    QBox2DRectItem *rightWall = new QBox2DRectItem();
    rightWall->setShape(rect);
    rightWall->setPos(200, -400);
    rightWall->setBrush(groundColor);
    rightWall->create(_world);
    emit itemCreated(rightWall);


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
        staticBox->create(_world);
        emit itemCreated(staticBox);
        b2RevoluteJointDef jd1;
        jd1.Initialize(staticBox->body(), ground->body(), staticBox->body()->GetPosition());
        jd1.motorSpeed = (float32)(pow(-1.0f,i) * 2 * b2_pi);
        jd1.maxMotorTorque = 50000000.0f;
        jd1.enableMotor = true;
        _world->CreateJoint(&jd1);
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
        item->create(_world);
        emit itemCreated(item);
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
        testItem->create(_world);
        emit itemCreated(testItem);
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
        item->create(_world);
        emit itemCreated(item);
    }
}
