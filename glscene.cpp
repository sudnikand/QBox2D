#include <QtOpenGL>
#include "glscene.h"

GLScene::GLScene(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    _alpha = 0;
    _beta = 0;
    _distance = -20;
    _scale = 20.0 / WORLD_SCALE_FACTOR;

    camera().setUpDirection(QVector3D(0, -1, 0));
    camera().setPosition(QVector3D(0, 0, _distance));
    //cameraTransformation.rotate(_alpha, 0, 1, 0);
    //cameraTransformation.rotate(_beta, 1, 0, 0);
}

GLScene::~GLScene()
{

}

void GLScene::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
}

    camera().projMatrix().setToIdentity();
    camera().projMatrix().ortho(-width, width, -height, height, 0.1, 10000 );
    //camera().projMatrix().perspective(60.0, (qreal)width/(qreal)height, 0.5f, 1000 );
    //pMatrix.rotate(90, QVector3D(0,0,1));
    //pMatrix.translate(QVector3D(0,-25,0));

    glViewport(0, 0, width, height);

}

void GLScene::initializeGL(){
    qglClearColor(Qt::black);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    _shader.addShaderFromSourceFile(QGLShader::Vertex,"data/shaders/texture.vsh");
    _shader.addShaderFromSourceFile(QGLShader::Fragment,"data/shaders/texture.fsh");
    _shader.link();

    emit initialized();
    qDebug() << "GL Scene initialized";
}

void GLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera().viewMatrix().setToIdentity();
    camera().lookAt(QVector3D(0, 0, 0));
    camera().viewMatrix().scale(_scale);

    QListIterator<QBox2DItem*> i(_glitems);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        item->draw();
    }
}

void GLScene::updateGL() {
    QGLWidget::updateGL();
}


QVector4D GLScene::unproject(const QVector3D & screen){
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    const qreal xNorm = (2.0f * ((screen.x() - viewport[0]) / (viewport[2] - viewport[0]))) - 1.0f;
    const qreal yNorm = 1.0f - (2.0f * ((screen.y() - viewport[1]) / (viewport[3] - viewport[1])));

    QMatrix4x4 pvMatrixInv = camera().projViewProduct().inverted();
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
//  Code below will work OK while projection is ortogonal
//  For some reason in perspective projection everything is flipped and not accurate
    QVector4D rNear = unproject(QVector3D(p.x(), p.y(), -1));
    QVector4D rFar =  unproject(QVector3D(p.x(), p.y(),  1));

    QVector<QVector3D> triangle;
    triangle << QVector3D(0, 0, 0) << QVector3D(0, 1, 0) << QVector3D(1, 0, 0);
    QVector3D normal = QVector3D::normal(triangle.at(0), triangle.at(1), triangle.at(2));

    qreal d1 = QVector3D::dotProduct(rNear.toVector3D() - triangle.at(0), normal);
    qreal d2 = QVector3D::dotProduct(rFar.toVector3D() - triangle.at(0), normal);

    QVector3D point = rNear.toVector3D() + (rFar.toVector3D() - rNear.toVector3D()) * (-d2 / (d2 - d1));
    return point.toPointF();
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
    item->_glscene = this;
    if (!item->textureName().isNull()) {
        if (!_textures.contains(item->textureName())) {
            qDebug() << "Loading texture: " << item->textureName();
            GLuint textureID = bindTexture(QPixmap("data/textures/" + item->textureName()), GL_TEXTURE_2D);
            _textures.insert(item->textureName(), textureID);
        }
    }
}

void GLScene::removeItem(QBox2DItem *item) {
    qDebug() << "Remove item: " << item->name();
    _glitems.removeOne(item);
}
void GLScene::zoomIn()   { _scale *= 1.1; }
void GLScene::zoomOut()  { _scale *= 0.9; }
void GLScene::clear()    { _glitems.clear(); }

GLCamera& GLScene::camera() {
    return _camera;
}

QHash<QString,GLuint>& GLScene::textures(){
    return _textures;
}

QGLShaderProgram* GLScene::shader(){
    return &_shader;
}

