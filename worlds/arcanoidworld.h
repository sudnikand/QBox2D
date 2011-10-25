#ifndef ARCANOIDWORLD_H
#define ARCANOIDWORLD_H

#include "world.h"

class ArcanoidWorld : public QBox2DWorld {

private:
    QBox2DRectItem*   _paddle;

public:
    ArcanoidWorld();
    void create(QGraphicsScene* const scene);
    void updateKeys(int key, int state);

};


#endif // ARCANOIDWORLD_H
