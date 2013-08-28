#ifndef GLSCENE_H
#define GLSCENE_H

#include "items.h"
#include <QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>


class GLCamera
{
public:
    void setPosition(const QVector3D &position){
        position_ = transformMatrix_ * position;
    }

    void setUpDirection(const QVector3D &upDirection){
        upDirection_ = transformMatrix_ * upDirection;
    }

    void lookAt(const QVector3D &center){
        viewMatrix_.lookAt(position_, center, upDirection_);
    }

    QMatrix4x4 projMatrix_;
    QMatrix4x4 viewMatrix_;
    QMatrix4x4 transformMatrix_;
    QVector3D  position_;
    QVector3D  upDirection_;
};

class GLScene : public QGLWidget
{
   Q_OBJECT

public:
    explicit GLScene(QWidget *parent=0);
    virtual ~GLScene();

public slots:
    void updateGL();
    void addItem(QBox2DItem *item);
    void removeItem(QBox2DItem *item);
    void zoomIn();
    void zoomOut();
    void clear();

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QPointF mapToScene(const QPointF &p);
    QVector4D unproject(const QVector3D &screen);
    QSize sizeHint() const;

signals:
    void mouseRightButtonPressed(const QPointF&);
    void mouseLeftButtonPressed(const QPointF&);
    void mouseRightButtonReleased();
    void mouseLeftButtonReleased();
    void mouseMoved(const QPointF&);
    void keyPressed(const int&);
    void keyReleased(const int&);
    void initialized();

private:
    QList<QBox2DItem*> _glitems;
    qreal              _scale;

    QGLShaderProgram shaderProgram;
    qreal _alpha, _beta, _distance;
    QHash<QString,GLuint> _textures;
    GLCamera camera;
};

#endif // GLSCENE_H
