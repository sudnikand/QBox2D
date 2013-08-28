#include "mainwindow.h"
#include "qscene.h"
#include "view.h"
#include "worlds.h"
#include "glscene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(1000/60);
    sound = new QSound("data/sounds/balloon.wav",this);
    sound->play();

    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("data/sounds/lullaby.ogg"));
    player->setVolume(90);
    player->play();

    startGame();
}

void MainWindow::createGLScene(){
    qDebug()<<"Creating GL Scene";
    glscene = new GLScene(this);
    ui->frameL->layout()->addWidget(glscene);

    connect(timer,SIGNAL(timeout()),glscene,SLOT(updateGL()));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), glscene, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), glscene, SLOT(zoomOut()));

    connect(glscene,SIGNAL(mouseLeftButtonPressed(QPointF)), world,SLOT(grabItem(QPointF)));
    connect(glscene,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(glscene,SIGNAL(mouseLeftButtonReleased()),       world,SLOT(dropItem()));
    connect(glscene,SIGNAL(mouseMoved(QPointF)),             world,SLOT(moveItem(QPointF)));
    connect(glscene,SIGNAL(keyPressed(int)),                 world,SLOT(handleKeyPressed(int)));
    connect(glscene,SIGNAL(keyReleased(int)),                world,SLOT(handleKeyReleased(int)));

    connect(world,SIGNAL(itemCreated(QBox2DItem*)),  glscene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),glscene,SLOT(removeItem(QBox2DItem*)));
    connect(world,SIGNAL(gameFinished()), this, SLOT(restartGame()));
    connect(glscene,SIGNAL(initialized()),world,SLOT(populate()));
}

void MainWindow::createQScene(){
    QScene *scene = new QScene(this);
    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setBackgroundBrush(Qt::white);
    //scene->setStickyFocus(false);

    QBox2DView *view = new QBox2DView(ui->frameR);
    view->setScene(scene);
    //view->fitInView(QRectF(0,0,100,100),Qt::KeepAspectRatioByExpanding);
    ui->frameR->layout()->addWidget(view);

    connect(timer,SIGNAL(timeout()),scene,SLOT(advance()));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), view, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), view, SLOT(zoomOut()));

    connect(view,SIGNAL(mouseLeftButtonPressed(QPointF)), world,SLOT(grabItem(QPointF)));
    connect(view,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(view,SIGNAL(mouseLeftButtonReleased()),       world,SLOT(dropItem()));
    connect(view,SIGNAL(mouseMoved(QPointF)),             world,SLOT(moveItem(QPointF)));
    connect(view,SIGNAL(keyPressed(int)),                 world,SLOT(handleKeyPressed(int)));
    connect(view,SIGNAL(keyReleased(int)),                world,SLOT(handleKeyReleased(int)));

    connect(world,SIGNAL(itemCreated(QBox2DItem*)),  scene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),scene,SLOT(removeItem(QBox2DItem*)));
}

void MainWindow::createWorld(){
    qDebug()<<"Creating World";
    //world = new TestWorld(this);
    //world = new ExampleWorld(this);
    world = new ArcanoidWorld(this);
    world->setSettings(1.0f / 60.0f, 10, 10);

    qDebug()<<"Connecting timer with World";
    connect(timer,SIGNAL(timeout()),world,SLOT(step()));

    qDebug()<<"Connecting world with sound";
    connect(world,SIGNAL(hit()),sound,SLOT(play()));
}

void MainWindow::deleteWorld(){
    glscene->clear();
    world->disconnect();
    delete world;
    world = NULL;
}

void MainWindow::deleteGLScene(){
    glscene->disconnect();
    delete glscene;
    glscene = NULL;
}

void MainWindow::startGame(){
    createWorld();
    createGLScene();
    timer->start();
    //_music->play();
}

void MainWindow::restartGame(){
    timer->disconnect();
    timer->stop();
    deleteWorld();
    deleteGLScene();
    startGame();
}

MainWindow::~MainWindow(){
    delete timer;
    delete world;
    delete ui;
    delete sound;
}
