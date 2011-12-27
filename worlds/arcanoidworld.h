#ifndef ARCANOIDWORLD_H
#define ARCANOIDWORLD_H

#include "world.h"

class ArcanoidWorld : public QBox2DWorld {

private:
    QBox2DRectItem*   _paddle;
    QBox2DRectItem*   _bound;
    QBox2DCircleItem* _ball;


public:
    ArcanoidWorld();
    void create(QGraphicsScene* const scene);
    void handleKeyPressed(const int &key);
    void step();

};


#endif // ARCANOIDWORLD_H
