#include "brick.h"

Brick::Brick() :
    QBox2DItem()
{
    _durability = 0;
    _textures.append("kde.png");
    _textures.append("face-smile.png");
    _textures.append("exit.png");
}

int Brick::handleContact(){
    _durability--;
    if (_durability >= 0)
        setTextureName(_textures.at(_durability));
    return _durability;
}

void Brick::setDurability(const int &d){
    if ( _textures.size() < d){
        setTextureName(_textures.last());
    } else {
        setTextureName(_textures.at(d));
    }
    _durability = d;
}
