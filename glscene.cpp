#include "glscene.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <QtGui/QApplication>

GLScene::GLScene(QWidget *parent) : QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

GLScene::~GLScene()
{

}

void GLScene::initializeGL(void){
    glClearColor(0,0,0,1.0f);

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

void GLScene::paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    begin2D(100,100);
    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(0,0,0);
    glColor3f(0,1,0);
    glVertex3f(50,0,0);
    glColor3f(0,0,1);
    glVertex3f(50,50,0);
    glEnd();
	end2D();
}

void GLScene::updateGL(void)
{
	QGLWidget::updateGL();
}

void GLScene::begin2D( int width, int height )
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
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

void GLScene::mouseMoveEvent(QMouseEvent *event)
{

}

void GLScene::mousePressEvent(QMouseEvent *event)
{

}

void GLScene::mouseReleaseEvent(QMouseEvent *event)
{

}

void GLScene::keyPressEvent(QKeyEvent *event)
{

}

void GLScene::keyReleaseEvent(QKeyEvent *event)
{

}