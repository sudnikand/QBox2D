#include "testworld.h"

TestWorld::TestWorld(QObject *parent) : QBox2DWorld(parent) {

}

void TestWorld::populate(){
    qDebug()<<"Creating TestWorld";
    loadWorld("data/levels/test.xml");
}

void TestWorld::step(){
    QBox2DWorld::step();
}
