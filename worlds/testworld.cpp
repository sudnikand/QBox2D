#include "testworld.h"

TestWorld::TestWorld() : QBox2DWorld() {
}

void TestWorld::create(){
    _world->SetGravity(b2Vec2(0, -10));
    QBox2DRectItem *ground = new QBox2DRectItem();
    QRectF groundrect(0, 0, 400, 5);
    QColor groundColor(64, 64, 64);
    ground->setShape(groundrect);
    ground->setPos(-200, 0);
    ground->setBrush(groundColor);
    ground->setBodyType(b2_staticBody);
    ground->create(_world);
    emit itemCreated(ground);
    _box = createBox(QPointF(0, -10));
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
        if (item && item->body()->GetType() != b2_staticBody ) {
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
    qDebug() << "handle Contact here";
    Q_UNUSED(cp);
}
