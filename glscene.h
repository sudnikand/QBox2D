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


public slots:
    void updateGL();
    void addItem(QBox2DItem *item)    { _glitems << item; }
    void removeItem(QBox2DItem *item) { _glitems.removeOne(item); }
    void scale(qreal s)               { _scale *= s; }
    void zoomIn()                     { scale(0.9); }
    void zoomOut()                    { scale(1.1); }

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void begin2D(int, int);
    void end2D();

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
};


#endif // GLCANVAS_H
