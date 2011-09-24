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
    timeStep = 1.0f / 60.0f;
    velocityIterations = 10;
    positionIterations = 10;
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(Qt::white);
    scene->setStickyFocus(false);
    scene->setSceneRect(-200, 20, 400, -500);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);

    createWorld();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(Step()));
    timer->start(1000/60);

}

void MainWindow::createWorld(){
    b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);

    // ground
    QBox2DRectItem *ground = new QBox2DRectItem();
    QRectF groundrect(0, 0, 400, 5);
    QColor groundColor(64, 64, 64);
    ground->setShape(groundrect);
    ground->setPos(-200, 0);
    ground->setBrush(groundColor);
    ground->create(world);
    scene->addItem(ground);

    // roof
    QBox2DRectItem *roof = new QBox2DRectItem();
    roof->setShape(groundrect);
    roof->setPos(-200, -405);
    roof->setBrush(groundColor);
    roof->create(world);
    scene->addItem(roof);

    //left wall
    QBox2DRectItem *leftWall = new QBox2DRectItem();
    QRectF rect(0, 0, 5, 400);
    leftWall->setShape(rect);
    leftWall->setPos(-205, -400);
    leftWall->setBrush(groundColor);
    leftWall->create(world);
    scene->addItem(leftWall);

    //right wall
    QBox2DRectItem *rightWall = new QBox2DRectItem();
    rightWall->setShape(rect);
    rightWall->setPos(200, -400);
    rightWall->setBrush(groundColor);
    rightWall->create(world);
    scene->addItem(rightWall);

    //add motors
    for (int i = 0; i < 2; ++i) {
        QBox2DRectItem *staticBox = new QBox2DRectItem();
        staticBox->setShape(QRectF(0, 0, 10, 50));
        staticBox->setPos(pow(-1.0f,i) * 60, -50);
        staticBox->setRotation(0);
        staticBox->setBrush(groundColor);
        staticBox->setBodyType(b2_dynamicBody);
        staticBox->setFriction(1.0f);
        staticBox->setDensity(2.0f);
        staticBox->setRestitution(0.1f);
        staticBox->create(world);
        scene->addItem(staticBox);
        b2RevoluteJointDef jd1;
        jd1.Initialize(staticBox->body(), ground->body(), staticBox->body()->GetPosition());
        jd1.motorSpeed = (float32)(pow(-1.0f,i) * 1 * b2_pi);
        jd1.maxMotorTorque = 50000000.0f;
        jd1.enableMotor = true;
        b2Joint* revJoint = world->CreateJoint(&jd1);
        //joints.append(revJoint);
    }

    //add falling boxes
    for (int i = 0; i < 50; ++i) {
        int size = 6 + qrand() % 10;
        QBox2DRectItem *item = new QBox2DRectItem();
        item->setShape(QRectF(0,0,size,size));
        item->setPos(-80 + qrand() % 40, -75 - qrand() % 150);
        item->setRotation(qrand() % 360);
        item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        item->setBodyType(b2_dynamicBody);
        item->setDensity(1.0f);
        item->setFriction(0.8f);
        item->setRestitution(0.4f);
        item->create(world);
        scene->addItem(item);
    }

    // add triangles
    for (int i = 0; i < 5; ++i) {
        int size = 4 + qrand() % 7;
        QPolygonF polygonForm;
        polygonForm << QPointF(0, -size*2) << QPointF(-size, 0) << QPointF(size, 0);
        QBox2DPolygonItem *testItem = new QBox2DPolygonItem();
        testItem->setShape(polygonForm);
        testItem->setPos(-20 + qrand() % 60, -50 - qrand() % 150);
        testItem->setRotation(qrand() % 360);
        testItem->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        testItem->setFriction(1.0f);
        testItem->setDensity(1.0f);
        testItem->setRestitution(0.5f);
        testItem->setBodyType(b2_dynamicBody);
        testItem->create(world);
        scene->addItem(testItem);
    }

    //Add circles
    for (int i = 0; i < 5; ++i) {
        int size = 3 + qrand() % 2;
        QBox2DCircleItem *item = new QBox2DCircleItem();
        item->setShape(QRectF(-size , -size, size*2, size*2));
        item->setPos(0 + qrand() % 60, -100 - qrand() % 150);
        item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        item->setBodyType(b2_dynamicBody);
        item->setFriction(0.1f);
        item->setDensity(1.0f);
        item->setRestitution(0.5f);
        item->create(world);
        scene->addItem(item);
    }
}

void MainWindow::Step() {
    world->Step(timeStep, velocityIterations, positionIterations);
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


MainWindow::~MainWindow()
{
    delete world;
    delete timer;
    delete scene;
    delete ui;
}
