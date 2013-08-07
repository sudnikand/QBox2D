#include "glscene.h"
#include <QtOpenGL>

GLScene::GLScene(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    _alpha = 0;
    _beta = 0;
    _distance = 50;
    _scale = 1.0;
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

    glAlphaFunc(GL_GREATER, 0.1f);
    glEnable(GL_ALPHA_TEST);

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex,"data/shaders/texture.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment,"data/shaders/texture.fsh");

    shaderProgram.link();

    _textures.append(bindTexture(QPixmap("data/textures/3d.png"),GL_TEXTURE_2D, GL_RGBA));
}

void GLScene::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
	}

    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (qreal)width/(qreal)height, 0.5f, 1000 );
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

    vMatrix.setToIdentity();
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);
    vMatrix.rotate(180, QVector3D(0,1,0));

    QVector<QVector2D> textureCoordinates;
    textureCoordinates << QVector2D(1, 1) << QVector2D(0, 1) << QVector2D(0, 0) << QVector2D(1, 0) ;

    QListIterator<QBox2DItem*> i(_glitems);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        shaderProgram.bind();
        shaderProgram.setUniformValue("mvpMatrix", pMatrix * vMatrix * item->_mMatrix);
        shaderProgram.setUniformValue("texture", 0);
        glBindTexture(GL_TEXTURE_2D, _textures.at(0));
        //shaderProgram.setUniformValue("color", item->color());
        shaderProgram.setAttributeArray("vertex", item->_vertices.constData());
        shaderProgram.enableAttributeArray("vertex");

        shaderProgram.setAttributeArray("textureCoordinate", textureCoordinates.constData());
        shaderProgram.enableAttributeArray("textureCoordinate");

        glDrawArrays(GL_TRIANGLE_FAN, 0, item->_vertices.size());
        shaderProgram.disableAttributeArray("vertex");
        shaderProgram.release();
    }

}

void GLScene::updateGL() {
    QGLWidget::updateGL();
}


QPointF GLScene::mapToScene(const QPointF &p){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    qDebug() << p << pMatrix << vMatrix;
    QMatrix4x4 pvMatrix = pMatrix * vMatrix;
    QMatrix4x4 transformMatrix = pvMatrix.inverted();

    const float unit_x = (2.0f*((float)(p.x()-viewport[0])/(viewport[2]-viewport[0])))-1.0f;
    const float unit_y = 1.0f-(2.0f*((float)(p.y()-viewport[1])/(viewport[3]-viewport[1])));

    //const float unit_x = (p.x()-viewport[0])/ viewport[2] * 2 - 1;
    //const float unit_y = (p.y()-viewport[1])/ viewport[3] * 2 - 1;

    QVector4D invec = QVector4D(unit_x,unit_y, 1, 1);
    const QVector4D near = transformMatrix * invec;

    QPointF point;
    point.setX(near.x() / near.w());
    point.setY(near.y() / near.w());
    qDebug() << invec << near << point;
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
