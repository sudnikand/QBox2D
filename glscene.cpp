#include "glscene.h"
#include <QtOpenGL>

GLScene::GLScene(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    _alpha = 0;
    _beta = 0;
    _distance = 50;
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

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex,"data/shaders/sqare.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment,"data/shaders/sqare.fsh");

    shaderProgram.link();
}

void GLScene::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
	}

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (qreal)width/(qreal)height, 0.5f, 100 );
    //pMatrix.translate(QVector3D(0,-25,0));

    glViewport(0, 0, width, height);

}

void GLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(_alpha, 0, 1, 0);
    cameraTransformation.rotate(_beta, 1, 0, 0);
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, _distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, -1, 0);

    QMatrix4x4 vMatrix;
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    vMatrix.rotate(180, QVector3D(0,1,0));

    QListIterator<QBox2DItem*> i(_glitems);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        shaderProgram.bind();
        shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * item->_mMatrix);
        shaderProgram.setUniformValue("color", item->color());
        shaderProgram.setAttributeArray("vertex", item->_vertices.constData());
        shaderProgram.enableAttributeArray("vertex");
        glDrawArrays(GL_TRIANGLE_FAN, 0, item->_vertices.size());
        shaderProgram.disableAttributeArray("vertex");
        shaderProgram.release();
    }

}

void GLScene::updateGL() {
    QGLWidget::updateGL();
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

QSize GLScene::sizeHint() const {
    return QSize(640,480);
}
