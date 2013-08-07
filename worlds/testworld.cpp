#include "testworld.h"

TestWorld::TestWorld(QObject *parent) : QBox2DWorld(parent) {

}

void TestWorld::populate(){
    qDebug()<<"Creating TestWorld";
    loadWorld("data/levels/test.xml");
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
