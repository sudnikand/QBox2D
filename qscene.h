#ifndef QSCENE_H
#define QSCENE_H

#include <QGraphicsScene>

class QScene : public QGraphicsScene
{
    Q_OBJECT
public:
    QScene(QObject* parent = 0);
public slots:
    void removeItem(QGraphicsItem *item);
    void addItem(QGraphicsItem* item);
};

#endif // QSCENE_H
