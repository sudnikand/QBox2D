#include "exampleworld.h"

ExampleWorld::ExampleWorld() : QBox2DWorld(){
}

void ExampleWorld::create(){
    _world->SetGravity(b2Vec2(0, 10));

    QColor groundColor(64, 64, 64);

    // ground
    QBox2DItem *ground = new QBox2DItem();
    ground->setPos(b2Vec2(0, 20));
    ground->createBody(_world);
    b2PolygonShape shape;
    shape.SetAsBox(20,1);
    ground->setShape(shape);
    ground->graphics()->setBrush(groundColor);
    appendItem(ground);

    // roof
    QBox2DItem *roof = new QBox2DItem();
    roof->setPos(b2Vec2(0, -20));
    roof->createBody(_world);
    roof->setShape(shape);
    roof->graphics()->setBrush(groundColor);
    appendItem(roof);

    //left wall
    QBox2DItem *leftWall = new QBox2DItem();
    leftWall->setPos(b2Vec2(-20, 0));
    leftWall->createBody(_world);
    shape.SetAsBox(1,20);
    leftWall->setShape(shape);
    leftWall->graphics()->setBrush(groundColor);
    appendItem(leftWall);

    //right wall
    QBox2DItem *rightWall = new QBox2DItem();
    rightWall->setPos(b2Vec2(20, 0));
    rightWall->createBody(_world);
    rightWall->setShape(shape);
    rightWall->graphics()->setBrush(groundColor);
    appendItem(rightWall);

    //add motors
    for (int i = 0; i < 2; ++i) {
        QBox2DItem *motor = new QBox2DItem();
        motor->setPos(b2Vec2(pow(-1.0f,i) * 5, 15));
        motor->setBodyType(b2_dynamicBody);
        motor->setDensity(2.0f);
        motor->setFriction(1.0f);
        motor->setRestitution(0.1f);
        motor->createBody(_world);
        shape.SetAsBox(0.5f, 4.0f);
        motor->setShape(shape);
        motor->graphics()->setBrush(groundColor);
        appendItem(motor);

        b2RevoluteJointDef jd1;
        jd1.Initialize(motor->body(), _ground, motor->body()->GetPosition());
        jd1.motorSpeed = -(float32)(pow(-1.0f,i) * 2 * b2_pi);
        jd1.maxMotorTorque = 50000000.0f;
        jd1.enableMotor = true;
        _world->CreateJoint(&jd1);
    }

    //add falling boxes
    for (int i = 0; i < 10; ++i) {
        QBox2DItem *box = new QBox2DItem();
        box->setPos(b2Vec2((qrand() % 20)-10.0f, -15.0f));
        box->setBodyType(b2_dynamicBody);
        box->setFriction(0.9f);
        box->setDensity(1.0f);
        box->setRestitution(0.5f);
        box->setRotation(qrand() % 360);
        box->createBody(_world);
        float32 size = ((qrand() % 3)+1.0f)/4;
        b2PolygonShape rect;
        rect.SetAsBox(size,size);
        box->setShape(rect);
        box->graphics()->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        appendItem(box);
    }

    // add triangles
    for (int i = 0; i < 10; ++i) {
        int size = Q2W_(4 + qrand() % 7);
        QBox2DItem *testItem = new QBox2DItem();

        testItem->setPos(b2Vec2(qrand() % 20 - 10.0f, -15.0f));
        testItem->setRotation(qrand() % 360);
        testItem->setFriction(1.0f);
        testItem->setDensity(5.0f);
        testItem->setRestitution(0.5f);
        testItem->setBodyType(b2_dynamicBody);
        testItem->createBody(_world);
        QVector<b2Vec2> vertex;
        vertex << b2Vec2(0,-size*2) << b2Vec2(-size,0) << b2Vec2(size,0);
        shape.Set(&vertex.toStdVector().at(0),vertex.size());
        testItem->setShape(shape);
        testItem->graphics()->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        appendItem(testItem);
    }



    for (int i = 0; i < 6; ++i) {

      QBox2DItem *ball = new QBox2DItem();
       ball->setBodyType(b2_dynamicBody);
   ball->setPos(b2Vec2(qrand() % 20-10, -10 + qrand() % 5));
       ball->setFriction(0.9f);
       ball->setDensity(1.0f);
       ball->setRestitution(0.5f);
       ball->createBody(_world);

       float32 radius = 0.3f;
       b2CircleShape circle;
       circle.m_radius = radius;
       ball->setShape(circle);
       ball->graphics()->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
       ball->body()->SetUserData(ball);
       appendItem(ball);    }
}
