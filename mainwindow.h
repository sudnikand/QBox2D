#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "world.h"

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
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void Step();
    void createWorld();
    void zoomIn();
    void zoomOut();

private:
    Ui::MainWindow*     ui;
    QGraphicsScene*     scene;
    QTimer*             timer;
    QBox2DWorld*        world;
};

#endif // MAINWINDOW_H
