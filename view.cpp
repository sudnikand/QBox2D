#include "view.h"
#include <QDebug>

QBox2DView::QBox2DView(QWidget* parent) : QGraphicsView(parent)
{
}

void QBox2DView::zoomIn() {
    scale(1.2,1.2);
}

void QBox2DView::zoomOut() {
    scale(0.8,0.8);
}

void QBox2DView::mousePressEvent(QMouseEvent *event) {
     if (event->button() == Qt::LeftButton) {
         QPointF pos = mapToScene(event->pos());
         emit mouseLeftButtonPressed(pos);
     } else
     if (event->button() == Qt::RightButton) {
         QPointF pos = mapToScene(event->pos());
         emit mouseRightButtonPressed(pos);
     }
}

void QBox2DView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit mouseRightButtonReleased();
    } else
    if (event->button() == Qt::LeftButton) {
        emit mouseLeftButtonReleased();
    }
}

void QBox2DView::mouseMoveEvent(QMouseEvent *event) {
    //if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) {
        QPointF pos = mapToScene(event->pos());
        emit mouseMoved(pos);
    //}
}

void QBox2DView::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event->key());
}

void QBox2DView::keyReleaseEvent(QKeyEvent *event) {
    emit keyReleased(event->key());
}
