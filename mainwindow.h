#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "world.h"
#include "view.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void createWorld();

private:
    Ui::MainWindow*     ui;
    QGraphicsScene*     scene;
    QTimer*             timer;
    QBox2DWorld*        world;
    QBox2DView*         view;
};

#endif // MAINWINDOW_H
