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
    qglClearColor(Qt::black);
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
    glOrtho(-width*_scale, width*_scale, height*_scale, -height*_scale, -1, 1);
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

QPointF GLScene::mapToScene(const QPointF &p){
    QPointF point;
    point.setY((p.y()*2 - height())*_scale);
    point.setX((p.x()*2 - width())*_scale);
    return point;
}

void GLScene::mousePressEvent(QMouseEvent *event) {
    QPointF pos = mapToScene(event->pos());

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
    QPointF pos = mapToScene(event->pos());
    emit mouseMoved(pos);
}

void GLScene::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event->key());
}

void GLScene::keyReleaseEvent(QKeyEvent *event) {
    emit keyReleased(event->key());
}
