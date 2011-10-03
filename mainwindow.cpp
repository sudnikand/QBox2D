#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::white);
    scene->setStickyFocus(false);
    scene->setSceneRect(-200, 30, 400, -500);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setViewport(new QGLWidget());
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);

    createWorld();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Step()));
    timer->start(1000/60);

}

void MainWindow::createWorld(){
    world = new ExampleWorld();
    world->setSettings(1.0f / 60.0f, 10, 10);
    world->create(scene);
}

void MainWindow::Step() {
    world->step();
    scene->advance();
    //QFuture<void> future1 = QtConcurrent::run(world, &b2World::Step, timeStep, velocityIterations, positionIterations);
    //QFuture<void> future2 = QtConcurrent::run(scene, &QGraphicsScene::advance);
    //future1.waitForFinished();
    //future2.waitForFinished();
}

void MainWindow::zoomIn() {
    ui->graphicsView->scale(1.2,1.2);
}

void MainWindow::zoomOut() {
    ui->graphicsView->scale(0.8,0.8);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF pos = ui->graphicsView->mapToScene(ui->graphicsView->mapFrom(this,event->pos()));
    QBox2DRectItem *box = new QBox2DRectItem();
    box->setBodyType(b2_dynamicBody);
    QRectF rect(0, 0, 10, 10);
    box->setShape(rect);
    box->setPos(pos.x()-10.0, pos.y()-10.0);
    box->setFriction(0.9f);
    box->setDensity(1.0f);
    box->setRestitution(0.5f);
    box->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    box->create(world);
    scene->addItem(box);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    world->updateKeys(event->key(),1);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    world->updateKeys(event->key(),0);
}

MainWindow::~MainWindow()
{
    delete world;
    delete timer;
    delete scene;
    delete ui;
}
