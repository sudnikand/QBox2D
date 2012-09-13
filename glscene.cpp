#include "glscene.h"
#include <QtOpenGL>

GLScene::GLScene(QWidget *parent) : QGLWidget(parent),
    _scale(1)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

GLScene::~GLScene()
{

}

void GLScene::initializeGL(){
    glClearColor(0,0,0,1.0f);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
}

void GLScene::resizeGL(int width, int height)
{
    if (height==0) {
        height=1;
	}

    glViewport(0, 0, width, height);
}

void GLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    begin2D(width(),height());

    int numItems = _glitems.size();
    for (int i = 0; i < numItems; ++i){
        _glitems.at(i)->render();
    }

    end2D();
}

void GLScene::updateGL() {
    QGLWidget::updateGL();
}

void GLScene::begin2D( int width, int height ){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-width*_scale, width*_scale, height*_scale, -height*_scale, -10, 10);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void GLScene::end2D( void )
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void GLScene::mousePressEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    qDebug("X: %f, Y: %f", pos.x(), pos.y());
    qDebug("W: %d, H: %d", width(), height());

    pos.setY(pos.y()*_scale);
    pos.setX(pos.x()*_scale);
    if (event->button() == Qt::LeftButton) {
         emit mouseLeftButtonPressed(pos);
    } else
    if (event->button() == Qt::RightButton) {
        emit mouseRightButtonPressed(pos);
    }
}

void GLScene::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit mouseRightButtonReleased();
    } else
    if (event->button() == Qt::LeftButton) {
        emit mouseLeftButtonReleased();
    }
}

void GLScene::mouseMoveEvent(QMouseEvent *event) {
    QPointF pos = event->pos();
    emit mouseMoved(pos);
}

void GLScene::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event->key());
}

void GLScene::keyReleaseEvent(QKeyEvent *event) {
    emit keyReleased(event->key());
}
