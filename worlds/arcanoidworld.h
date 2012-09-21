#ifndef ARCANOIDWORLD_H
#define ARCANOIDWORLD_H

#include "world.h"

class ArcanoidWorld : public QBox2DWorld
{

private:
    QBox2DItem*   _paddle;
    QBox2DItem*   _bound;
    QBox2DItem*   _ball;

public:
    ArcanoidWorld();
    void populate();
    void createBall(float32 radius);
    void createBall();
    void handleKeyPressed(const int &key);
    void step();

};


#endif // ARCANOIDWORLD_H
