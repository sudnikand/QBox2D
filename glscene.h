#ifndef GLSCENE_H
#define GLSCENE_H

#include "glitem.h"
#include <QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

class GLScene : public QGLWidget
{
   Q_OBJECT

public:
    GLScene(QWidget *parent=0);
    virtual ~GLScene();
    void updateGL();
    void clear() { _glitems.clear(); }

public slots:
    void addItem(GLItem *item) { _glitems << item; }
    void removeItem(GLItem *item) { _glitems.removeOne(item); }

protected:
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();

    void begin2D(int, int);
    void end2D();

	void mouseMoveEvent(QMouseEvent *pEvent);
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);

	void keyPressEvent(QKeyEvent *pEvent);
	void keyReleaseEvent(QKeyEvent *pEvent);

private:
    QList<GLItem*> _glitems;
};


#endif // GLCANVAS_H
