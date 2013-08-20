#ifndef BRICK_H
#define BRICK_H

#include "items.h"

class Brick : public QBox2DItem
{
public:
    Brick();
       
private:
    int _maxDamage;
    int _currentDamage;
};

#endif // BRICK_H
