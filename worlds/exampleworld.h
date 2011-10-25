#ifndef EXAMPLEWORLD_H
#define EXAMPLEWORLD_H

#include "world.h"

class ExampleWorld : public QBox2DWorld {
public:
    ExampleWorld();
    void create(QGraphicsScene* const scene);
};


#endif // EXAMPLEWORLD_H
