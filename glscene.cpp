#include "glscene.h"
#include <QtOpenGL>

GLScene::GLScene(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    _alpha = 0;
    _beta = 0;
    _distance = -50;
    _scale = 20.0 / WORLD_SCALE_FACTOR;

    camera.setUpDirection(QVector3D(0, -1, 0));
    camera.setPosition(QVector3D(0, 0, _distance));
    //cameraTransformation.rotate(_alpha, 0, 1, 0);
    //cameraTransformation.rotate(_beta, 1, 0, 0);
}

GLScene::~GLScene()
{

}

void GLScene::initializeGL(){
    qglClearColor(Qt::black);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram.addShaderFromSourceFile(QGLShader::Vertex,"data/shaders/texture.vsh");
    shaderProgram.addShaderFromSourceFile(QGLShader::Fragment,"data/shaders/texture.fsh");
    shaderProgram.link();

    emit initialized();
    qDebug() << "GL Scene initialized";
}

void GLScene::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
}

    camera.projMatrix_.setToIdentity();
    camera.projMatrix_.ortho(-width, width, -height, height, 0.1, 100 );
    //pMatrix.perspective(90.0, (qreal)width/(qreal)height, 0.5f, 1000 );
    //pMatrix.rotate(90, QVector3D(0,0,1));
    //pMatrix.translate(QVector3D(0,-25,0));

    glViewport(0, 0, width, height);

}

void GLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.viewMatrix_.setToIdentity();
    camera.lookAt(QVector3D(0, 0, 0));
    camera.viewMatrix_.scale(_scale);

    QVector<QVector2D> textureCoordinates;
    textureCoordinates << QVector2D(0, 1) << QVector2D(0, 0) << QVector2D(1, 0) << QVector2D(1, 1);

    shaderProgram.bind();
    QListIterator<QBox2DItem*> i(_glitems);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        shaderProgram.setUniformValue("modelMatrix", item->modelMatrix());
        shaderProgram.setUniformValue("viewMatrix", camera.viewMatrix_);
        shaderProgram.setUniformValue("projMatrix", camera.projMatrix_);
        shaderProgram.setUniformValue("texture", 0);
        glBindTexture(GL_TEXTURE_2D, _textures.value(item->textureName()));
        //shaderProgram.setUniformValue("color", item->color());
        shaderProgram.setAttributeArray("vertex", item->vertices().constData());
        shaderProgram.enableAttributeArray("vertex");

        shaderProgram.setAttributeArray("textureCoordinate", textureCoordinates.constData());
        shaderProgram.enableAttributeArray("textureCoordinate");

        glDrawArrays(GL_TRIANGLE_FAN, 0, item->vertices().size());
        shaderProgram.disableAttributeArray("vertex");
    }
    shaderProgram.release();

}

void GLScene::updateGL() {
    QGLWidget::updateGL();
}


QVector4D GLScene::unproject(const QVector3D & screen){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const qreal xNorm = (2.0f * ((screen.x() - viewport[0]) / (viewport[2] - viewport[0]))) - 1.0f;
    const qreal yNorm = 1.0f - (2.0f * ((screen.y() - viewport[1]) / (viewport[3] - viewport[1])));

    QMatrix4x4 pvMatrixInv = (camera.projMatrix_ * camera.viewMatrix_).inverted();
    QVector4D worldPoint = pvMatrixInv * QVector4D(xNorm, yNorm, screen.z(), 1);

    if (worldPoint.w() == 0){
        return QVector4D(0,0,0,0);
    }

    worldPoint.setW(1 / worldPoint.w());
    worldPoint.setX(worldPoint.x() * worldPoint.w());
    worldPoint.setY(worldPoint.y() * worldPoint.w());

    return worldPoint;
}

QPointF GLScene::mapToScene(const QPointF &p){
// #TODO: create ray from near plane to far plane,
//        passing Z [-1, 1] coordinate to unproject(),
//        code below will work OK while projection is ortogonal
    return unproject(QVector3D(p)).toPointF();
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

void GLScene::addItem(QBox2DItem *item)    {
    //qDebug() << "Add item: " << item->name();
    _glitems << item;
    if (!item->textureName().isNull()) {
        if (!_textures.contains(item->textureName())) {
            qDebug() << "Loading texture: " << item->textureName();
            _textures.insert(item->textureName(), bindTexture(QPixmap("data/textures/" + item->textureName()), GL_TEXTURE_2D));
        }
    }
}
void GLScene::removeItem(QBox2DItem *item) {
    qDebug() << "Remove item: " << item->name();
    _glitems.removeOne(item);
}
void GLScene::zoomIn()                     { _scale *= 1.1; }
void GLScene::zoomOut()                    { _scale *= 0.9; }
void GLScene::clear()                      { _glitems.clear(); }
