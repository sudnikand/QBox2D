#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <Box2D/Box2D.h>
#include "qbox2ditems.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);

public slots:
    void Step();
    void createWorld();
    void zoomIn();
    void zoomOut();

private:
    Ui::MainWindow*     ui;
    QGraphicsScene*     scene;
    QTimer*             timer;
    b2World*            world;
    float32             timeStep;
    int32               velocityIterations;
    int32               positionIterations;
    //QVector<b2Joint*>   joints;
};

#endif // MAINWINDOW_H
