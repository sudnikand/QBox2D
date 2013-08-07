#ifndef GLSCENE_H
#define GLSCENE_H

#include "items.h"
#include <QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

class GLScene : public QGLWidget
{
   Q_OBJECT

public:
    GLScene(QWidget *parent=0);
    virtual ~GLScene();
    void clear() { _glitems.clear(); }
    QPointF mapToScene(const QPointF &p);
    QSize sizeHint() const;

public slots:
    void updateGL();
    void addItem(QBox2DItem *item)    { _glitems << item; }
    void removeItem(QBox2DItem *item) { _glitems.removeOne(item); }
    void scale(qreal s)               { _scale *= s; }
    void zoomIn()                     { _distance *= 0.9; }
    void zoomOut()                    { _distance *= 1.1; }

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    void mouseRightButtonPressed(const QPointF&);
    void mouseLeftButtonPressed(const QPointF&);
    void mouseRightButtonReleased();
    void mouseLeftButtonReleased();
    void mouseMoved(const QPointF&);
    void keyPressed(const int&);
    void keyReleased(const int&);

private:
    QList<QBox2DItem*> _glitems;
    qreal              _scale;

    QMatrix4x4 pMatrix;
    QMatrix4x4 vMatrix;
    QGLShaderProgram shaderProgram;
    qreal _alpha, _beta, _distance;
    QVector<GLuint> _textures;
};


#endif // GLCANVAS_H
