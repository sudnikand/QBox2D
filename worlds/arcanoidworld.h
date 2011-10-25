#ifndef ARCANOIDWORLD_H
#define ARCANOIDWORLD_H

#include "world.h"

class ArcanoidWorld : public QBox2DWorld {

private:
    QBox2DRectItem*   _paddle;
    QBox2DCircleItem* _ball;

public:
    ArcanoidWorld();
    void create(QGraphicsScene* const scene);
    void updateKeys(int key, int state);
    void step();

};


#endif // ARCANOIDWORLD_H
