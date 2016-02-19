#ifndef GLSCENE_H
#define GLSCENE_H

#include <QGLWidget>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include "items.h"
#include "glcamera.h"

class GLScene : public QGLWidget
{
   Q_OBJECT

public:
    explicit GLScene(QWidget *parent=0);
    virtual ~GLScene();

    GLCamera& camera();
    QHash<QString,GLuint>& textures();
    QGLShaderProgram* shader();
    QString               _shader_dir;
    QString               _texture_dir;


public slots:
    void updateGL();
    void addItem(QBox2DItem *item);
    void removeItem(QBox2DItem *item);
    void zoomIn();
    void zoomOut();
    void clear();

private:
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
    qreal _alpha, _beta, _distance, _scale;
    GLCamera              _camera;
    QGLShaderProgram      _shader;
    QHash<QString,GLuint> _textures;
    QList<QBox2DItem*>    _glitems;
};

#endif // GLSCENE_H
