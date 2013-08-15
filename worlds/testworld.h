#ifndef TESTWORLD_H
#define TESTWORLD_H

#include "world.h"

class TestWorld : public QBox2DWorld {

private:
    QBox2DItem* _box;

public:
    TestWorld(QObject* parent = 0);
    void populate();
    void step();
};
#endif // TESTWORLD_H
