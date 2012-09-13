#include "world.h"

QBox2DWorld::QBox2DWorld(QObject* parent): QObject(parent),
    _mouseJoint(NULL) {
    _world = new b2World(b2Vec2(0,0));
    _world->SetContactListener(this);
    b2BodyDef bd;
    _ground = _world->CreateBody(&bd);
}

QBox2DWorld::~QBox2DWorld() {
    delete _world;
    _world = NULL;
}

void QBox2DWorld::setSettings(float32 timeStep, int32 velIters, int32 posIters){
    _timeStep = timeStep;
    _velocityIterations = velIters;
    _positionIterations = posIters;
    _world->SetWarmStarting(true);
    _world->SetContinuousPhysics(true);
}

void QBox2DWorld::handleKeyPressed(const int &key)
{
    Q_UNUSED(key);
}

void QBox2DWorld::handleKeyReleased(const int &key)
{
    Q_UNUSED(key);
}

void handleContact(const ContactPoint &cp)
{
    Q_UNUSED(cp);
}

void QBox2DWorld::step(){
    _contacts.clear();

    QListIterator<QBox2DItem*> i(_items);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        item->update();
    }

    _world->Step(_timeStep,_velocityIterations,_positionIterations);
}

QBox2DItem* QBox2DWorld::createBox(const QPointF& pos) {
    QBox2DItem *box = new QBox2DItem();
    box->setBodyType(b2_dynamicBody);
    box->setFriction(0.9f);
    box->setDensity(1.0f);
    box->setRestitution(0.5f);
    box->setPos(b2Vec2(Q2W(pos.x(), pos.y())));
    box->createBody(_world);
    float32 l = 1.0f;
    b2PolygonShape rect;
    rect.SetAsBox(l/2,l/2);
    box->setShape(rect);
    box->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    box->body()->SetUserData(box);
    appendItem(box);
    qDebug() << "Box created";
    return box;
}

void QBox2DWorld::grabItem(const QPointF &p) {

    if (_mouseJoint != NULL){
        return;
    }

    b2Vec2 pos( Q2W(p.x(), p.y()) );

    // Make a small box.
    b2AABB aabb;
    b2Vec2 d(0.001f, 0.001f);
    aabb.lowerBound = pos - d;
    aabb.upperBound = pos + d;

    // Query the world for overlapping shapes.

    QueryCallback callback(pos);
    _world->QueryAABB(&callback, aabb);

    if (callback._fixture) {
        b2Body* body = callback._fixture->GetBody();
        b2MouseJointDef md;
        md.bodyA = _ground;
        md.bodyB = body;
        md.target = pos;
        md.maxForce = 1000.0f * body->GetMass();
        _mouseJoint = (b2MouseJoint*) _world->CreateJoint(&md);
        body->SetAwake(true);
    }
}

void QBox2DWorld::dropItem(){
    if(_mouseJoint){
        _world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
}

void QBox2DWorld::moveItem(const QPointF &p){
    if(_mouseJoint){
        _mouseJoint->SetTarget(b2Vec2(Q2W(p.x(),p.y())));
    }
}

void QBox2DWorld::handleContact(const ContactPoint &cp){
    Q_UNUSED(cp);
}

void QBox2DWorld::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
    const b2Manifold* manifold = contact->GetManifold();

    if (manifold->pointCount == 0){
        return;
    }

    b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
    b2GetPointStates(state1, state2, oldManifold, manifold);

    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    for (int i = 0; i < manifold->pointCount && _contacts.size() < 1000; ++i){
        ContactPoint cp;
        cp.itemA = static_cast<QBox2DItem*>(contact->GetFixtureA()->GetBody()->GetUserData());
        cp.itemB = static_cast<QBox2DItem*>(contact->GetFixtureB()->GetBody()->GetUserData());
        cp.position = worldManifold.points[i];
        cp.normal = worldManifold.normal;
        cp.state = state2[i];
        _contacts.append(cp);
    }
}

void QBox2DWorld::destroyItem(QBox2DItem *item)
{
    _world->DestroyBody(item->body());
    removeItem(item);
    delete item;
    item = NULL;
}

void QBox2DWorld::appendItem(QBox2DItem *item){
    _items.append(item);
    emit itemCreated(item);
}

void QBox2DWorld::removeItem(QBox2DItem *item){
    _items.removeOne(item);
    emit itemDestroyed(item);
}
