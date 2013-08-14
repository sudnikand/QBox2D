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
    QVector4D unproject(const QVector3D &screen);
    QSize sizeHint() const;

public slots:
    void updateGL();
    void addItem(QBox2DItem *item)    {
        _glitems << item;
        if (!item->_textureName.isNull()) {
            if (!_textures.contains(item->_textureName)) {
                qDebug() << "Loading texture: " << item->_textureName;
                _textures.insert(item->_textureName, bindTexture(QPixmap("data/textures/" + item->_textureName), GL_TEXTURE_2D));
            }
        }
    }
    void removeItem(QBox2DItem *item) { _glitems.removeOne(item); }
    void scale(qreal s)               { _scale *= s; }
    void zoomIn()                     { _scale *= 1.1; }
    void zoomOut()                    { _scale *= 0.9; }

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
    void initialized();

private:
    QList<QBox2DItem*> _glitems;
    qreal              _scale;

    QMatrix4x4 pMatrix;
    QMatrix4x4 vMatrix;
    QGLShaderProgram shaderProgram;
    qreal _alpha, _beta, _distance;
    QHash<QString,GLuint> _textures;
};


#endif // GLCANVAS_H
