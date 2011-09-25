#include "qbox2ditems.h"

void QBox2DRectItem::create(b2World* const world){
    createBody(world);
    b2PolygonShape shape;
    shape.SetAsBox(Q2W_(_rect.width())/2, Q2W_(_rect.height())/2,
                      b2Vec2( Q2W(_rect.center().x(),-_rect.center().y()) ), 0);
    _fd.shape = &shape;
    _body->CreateFixture(&_fd);
}

void QBox2DRectItem::setShape(const QRectF& rect) {
    _rect = rect;
    _boundingRect = QRectF();
    update();
}

void QBox2DRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(_brush);
    painter->setPen(_pen);
    painter->drawRect(_rect);
}

QRectF QBox2DRectItem::boundingRect() const {
    if (_boundingRect.isNull()) {
        qreal halfpw = _pen.widthF() / 2;
        _boundingRect = _rect;
        if (halfpw > 0.0)
            _boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return _boundingRect;
}

void QBox2DCircleItem::create(b2World* const world){
    createBody(world);
    b2CircleShape shape;
    shape.m_radius = Q2W_(_rect.width()) / 2;
    _fd.shape = &shape;
    _body->CreateFixture(&_fd);
}

void QBox2DCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(_brush);
    painter->setPen(_pen);
    painter->drawEllipse(_rect);
}

void QBox2DCircleItem::setShape(const QRectF& rect) {
    _rect = rect;
    _boundingRect = QRectF();
    update();
}

QRectF QBox2DCircleItem::boundingRect() const {
    if (_boundingRect.isNull()) {
        qreal pw = _pen.widthF();
        if (pw == 0.0)
            _boundingRect = _rect;
        else
            _boundingRect = shape().controlPointRect();
    }
    return _boundingRect;
}


void QBox2DPolygonItem::create(b2World* const world) {
    createBody(world);
    const int n = _polygon.size();
    b2Vec2* vertices = new b2Vec2[n];
    for (int i = 0; i < n; ++i) {
        const QPointF &p = _polygon.at(i);
        vertices[i].Set( Q2W(p.x(), -p.y()) );
    }
    b2PolygonShape shape;
    shape.Set(vertices, n);
    delete[] vertices;
    vertices = NULL;
    _fd.shape = &shape;
    _body->CreateFixture(&_fd);
}

void QBox2DPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setBrush(_brush);
    painter->setPen(_pen);
    painter->drawPolygon(_polygon);
}

QRectF QBox2DPolygonItem::boundingRect() const {
    if (_boundingRect.isNull()) {
        qreal pw = _pen.widthF();
        if (pw == 0.0)
            _boundingRect = _polygon.boundingRect();
        else
            _boundingRect = _polygon.boundingRect().adjusted(-pw, -pw, pw, pw);
        }
    return _boundingRect;
}

void QBox2DPolygonItem::setShape(const QPolygonF& polygon) {
    _polygon.clear();
    _polygon = polygon;
    _boundingRect = QRectF();
    update();
}
