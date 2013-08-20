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
    ArcanoidWorld(QObject* parent = 0);
    virtual ~ArcanoidWorld();
    void populate();
    void handleKeyPressed(const int &key);
    void step();

private:
    void createBall(float32 radius);
    void adjustBallSpeed();
};


#endif // ARCANOIDWORLD_H
