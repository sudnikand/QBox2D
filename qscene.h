#ifndef QSCENE_H
#define QSCENE_H

#include "items.h"
#include <QGraphicsScene>

class QScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QScene(QObject* parent = 0);
public slots:
    void removeItem(QBox2DItem *item);
    void addItem(QBox2DItem *item);
};

#endif // QSCENE_H
