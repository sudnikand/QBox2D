#include "testworld.h"

TestWorld::TestWorld() : QBox2DWorld() {
}

void TestWorld::create(){
    _world->SetGravity(b2Vec2(0, 10));
    {
    QBox2DItem *ground = new QBox2DItem();
    ground->setPos(b2Vec2(0, 0));
    ground->createBody(_world);
    b2PolygonShape shape;
    shape.SetAsBox(20,1);
    ground->setShape(shape);
    appendItem(ground);
    ground->graphics()->setBrush(QColor(64,80,64));
    ground->body()->SetUserData(ground);

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

    b2CircleShape circle;
    circle.m_radius = radius;
    ball->setShape(circle);
    appendItem(ball);
    ball->graphics()->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    ball->body()->SetUserData(ball);
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
