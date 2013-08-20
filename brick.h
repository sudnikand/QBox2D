#ifndef BRICK_H
#define BRICK_H

#include "items.h"

class Brick : public QBox2DItem
{
public:
    Brick();
    int handleContact();
    void setDurability(const int&);
       
private:
    int _durability;
    QVector<QString> _textures;
};

#endif // BRICK_H
