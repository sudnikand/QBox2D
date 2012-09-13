#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::white);
    scene->setStickyFocus(false);

    glscene = new GLScene(this);

    view = new QBox2DView(ui->centralWidget);
    view->setScene(scene);
    connect(ui->actionZoomIn, SIGNAL(triggered()), view, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), view, SLOT(zoomOut()));
    ui->horizontalLayout->addWidget(view);
    ui->horizontalLayout->addWidget(glscene);
    view->fitInView(QRectF(0,0,100,100),Qt::KeepAspectRatioByExpanding);

    createWorld();

    timer = new QTimer(this);
    connect(view,SIGNAL(mouseLeftButtonPressed(QPointF)),world,SLOT(grabItem(QPointF)));
    connect(view,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(view,SIGNAL(mouseLeftButtonReleased()),world,SLOT(dropItem()));
    connect(view,SIGNAL(mouseMoved(QPointF)),world,SLOT(moveItem(QPointF)));
    connect(view,SIGNAL(keyPressed(int)),world,SLOT(handleKeyPressed(int)));
    connect(view,SIGNAL(keyReleased(int)),world,SLOT(handleKeyReleased(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));

    timer->start(1000/60);

}

void MainWindow::createWorld(){
    //world = new TestWorld();
    //world = new ExampleWorld();
    world = new ArcanoidWorld();

    connect(world,SIGNAL(itemCreated(QBox2DItem*)),scene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),scene,SLOT(removeItem(QBox2DItem*)));
    connect(world,SIGNAL(itemCreated(QBox2DItem*)),glscene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),glscene,SLOT(removeItem(QBox2DItem*)));

    world->setSettings(1.0f / 60.0f, 10, 10);
    world->create();
}

void MainWindow::update(){
    world->step();
    scene->advance();
    glscene->updateGL();
}

MainWindow::~MainWindow(){
    delete timer;
    delete world;
    delete view;
    delete scene;
    delete ui;
}
