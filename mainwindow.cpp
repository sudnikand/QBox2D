#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::white);
    scene->setStickyFocus(false);

    view = new QBox2DView(ui->centralWidget);
    view->setScene(scene);
    view->setViewport(new QGLWidget());
    connect(ui->actionZoomIn, SIGNAL(triggered()), view, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), view, SLOT(zoomOut()));
    ui->horizontalLayout->addWidget(view);

    createWorld();

    timer = new QTimer(this);
    connect(view,SIGNAL(mouseLeftButtonPressed(QPointF)),world,SLOT(grabItem(QPointF)));
    connect(view,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(view,SIGNAL(mouseLeftButtonReleased()),world,SLOT(dropItem()));
    connect(view,SIGNAL(mouseMoved(QPointF)),world,SLOT(moveItem(QPointF)));
    connect(view,SIGNAL(keyPressed(int)),world,SLOT(handleKeyPressed(int)));
    connect(view,SIGNAL(keyReleased(int)),world,SLOT(handleKeyReleased(int)));
    connect(timer,SIGNAL(timeout()),world,SLOT(step()));
    timer->start(1000/60);

}

void MainWindow::createWorld(){
    //world = new ExampleWorld();
    //world = new TestWorld();
    world = new ArcanoidWorld();
    world->setSettings(1.0f / 60.0f, 10, 10);
    world->create(scene);
}

MainWindow::~MainWindow(){
    delete timer;
    delete world;
    delete view;
    delete scene;
    delete ui;
}
