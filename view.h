#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QMouseEvent>

class QBox2DView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QBox2DView(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

public slots:
    void zoomIn();
    void zoomOut();

signals:
    void mouseRightButtonPressed(const QPointF&);
    void mouseLeftButtonPressed(const QPointF&);
    void mouseRightButtonReleased();
    void mouseLeftButtonReleased();
    void mouseMoved(const QPointF&);
    void keyPressed(const int&);
    void keyReleased(const int&);

};

#endif // VIEW_H
