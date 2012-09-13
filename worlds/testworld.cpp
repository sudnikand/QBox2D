#include "testworld.h"

TestWorld::TestWorld() : QBox2DWorld() {
}

void TestWorld::create(){
    qDebug()<<"Creating TestWorld";
    _world->SetGravity(b2Vec2(0, 10));
    {
    QBox2DItem *ground = new QBox2DItem();
    ground->setPos(b2Vec2(0, 0));
    ground->createBody(_world);
    ground->body()->SetUserData(ground);
    b2PolygonShape shape;
    shape.SetAsBox(20,1);
    ground->setShape(shape);
    ground->setColor(QColor(64,80,64));
    appendItem(ground);
    }

    {
    QBox2DItem *ball = new QBox2DItem();
    ball->setBodyType(b2_dynamicBody);
    float32 radius = 1.0f;
    ball->setPos(b2Vec2(Q2W(0, -200)));
    ball->setFriction(0.9f);
    ball->setDensity(1.0f);
    ball->setRestitution(0.5f);
    ball->createBody(_world);
    ball->body()->SetUserData(ball);

    b2CircleShape circle;
    circle.m_radius = radius;
    ball->setShape(circle);
    ball->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    appendItem(ball);
    }
}

void TestWorld::step(){
    QBox2DWorld::step();

    if (_contacts.size() == 0) {
        return;
    }

    QSet<QBox2DItem*> dest_items;
    for(int i = 0; i < _contacts.size() ; ++i){
        ContactPoint cp = _contacts.at(i);
        QBox2DItem *item = ( cp.itemA == _box ) ? cp.itemB :
                           ( cp.itemB == _box ) ? cp.itemA : NULL;
        if (item && item->bodyType() != b2_staticBody ) {
            dest_items.insert(item);
        }
    }

    QSetIterator<QBox2DItem*> i(dest_items);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        destroyItem(item);
    }

}

void TestWorld::handleContact(const ContactPoint &cp){
    Q_UNUSED(cp);
}
