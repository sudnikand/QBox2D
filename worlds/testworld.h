#ifndef TESTWORLD_H
#define TESTWORLD_H

#include "world.h"

class TestWorld : public QBox2DWorld {

private:
    QBox2DRectItem* _box;

public:
    TestWorld();
    void create(QGraphicsScene* const scene);
    void handleContact(const ContactPoint &cp);
    void step();
};
#endif // TESTWORLD_H
