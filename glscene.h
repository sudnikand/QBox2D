#ifndef GLSCENE_H
#define GLSCENE_H

#include <QGLWidget>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include <QtCore/QTimer>

class GLScene : public QGLWidget
{
   Q_OBJECT

public:
    GLScene(QWidget *parent=0);
    virtual ~GLScene();
    void updateGL(void);

protected:
	void initializeGL(void);
    void resizeGL(int, int);
	void paintGL(void);

    void begin2D(int, int);
	void end2D( void );

	void mouseMoveEvent(QMouseEvent *pEvent);
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);

	void keyPressEvent(QKeyEvent *pEvent);
	void keyReleaseEvent(QKeyEvent *pEvent);
};


#endif // GLCANVAS_H
