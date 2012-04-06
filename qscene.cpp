#include "qscene.h"
#include <QDebug>

QScene::QScene(QObject * parent) :
    QGraphicsScene(parent)
{

}

void QScene::removeItem(QGraphicsItem *item){
    QGraphicsScene::removeItem(item);
}

void QScene::addItem(QGraphicsItem* item){
    QGraphicsScene::addItem(item);
}
