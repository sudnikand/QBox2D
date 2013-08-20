#include "exampleworld.h"

ExampleWorld::ExampleWorld(QObject *parent) : QBox2DWorld(parent) {

}

void ExampleWorld::populate(){
    loadWorld("data/levels/motors.xml");
    b2PolygonShape shape;

    //add falling boxes
    for (int i = 0; i < 10; ++i) {
        QBox2DItem *box = new QBox2DItem();
        box->setPos(b2Vec2(WSCALE2((qrand() % 10 - 5), -15.0f)));
        box->setBodyType(b2_dynamicBody);
        box->setFriction(0.9f);
        box->setDensity(1.0f);
        box->setRestitution(0.5f);
        box->setRotation(qrand() % 360);
        box->createBody(_world);
        float32 size = WSCALE(((qrand() % 30)+30.0f)/40);
        b2PolygonShape rect;
        rect.SetAsBox(size,size);
        box->setShape(rect);
        box->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        box->setTextureName("exit.png");
        box->body()->SetUserData(box);
        appendItem(box);
    }

    // add triangles
    for (int i = 0; i < 10; ++i) {
        float32 size = WSCALE(((qrand() % 30) + 10.0f)/20);
        QBox2DItem *testItem = new QBox2DItem();

        testItem->setPos(b2Vec2(WSCALE2(qrand() % 20 - 10.0f, -15.0f)));
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
        testItem->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        testItem->setTextureName("kde.png");
        testItem->body()->SetUserData(testItem);
        appendItem(testItem);
    }



    for (int i = 0; i < 50; ++i) {
        QBox2DItem *ball = new QBox2DItem();
        ball->setBodyType(b2_dynamicBody);
        ball->setPos(b2Vec2(WSCALE2((qrand() % 30)/10.0f - 1.5f, (qrand() % 10)/10.0f - 1.5f)));
        ball->setFriction(0.1f);
        ball->setDensity(1.0f);
        ball->setRestitution(0.5f);
        ball->createBody(_world);

        float32 radius = WSCALE(1.1f);
        b2CircleShape circle;
        circle.m_radius = radius;
        ball->setShape(circle);
        ball->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        ball->body()->SetUserData(ball);
        if (qrand() % 2) {
            ball->setTextureName("face-smile.png");
        } else {
            ball->setTextureName("face-surprise.png");
        }
        appendItem(ball);
    }
}
