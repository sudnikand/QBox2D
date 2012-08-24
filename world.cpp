#include "world.h"

QBox2DWorld::QBox2DWorld(QObject* parent): QObject(parent),
    _mouseJoint(NULL) {
    _world = new b2World(b2Vec2(0,0));
    _world->SetContactListener(this);
    b2BodyDef bd;
    _groundBody = _world->CreateBody(&bd);
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
    _world->Step(_timeStep,_velocityIterations,_positionIterations);
}

QBox2DRectItem* QBox2DWorld::createBox(const QPointF& pos) {
    QBox2DRectItem *box = new QBox2DRectItem();
    box->setBodyType(b2_dynamicBody);
    uint l = 10;
    box->setPos(pos.x()-l, pos.y()-l);
    box->setFriction(0.9f);
    box->setDensity(1.0f);
    box->setRestitution(0.5f);
    box->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    box->create(_world);
    QRectF rect(0, 0, l, l);
    box->setShape(rect);
    emit itemCreated(box);
    return box;
}

void QBox2DWorld::grabItem(const QPointF &p) {

    if (_mouseJoint != NULL){
        return;
    }

    b2Vec2 pos( Q2W(p.x(), -p.y()) );

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
        md.bodyA = _groundBody;
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
        b2Vec2 pos;
        pos.x =  Q2W_(p.x());
        pos.y = -Q2W_(p.y());
        _mouseJoint->SetTarget(pos);
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
    emit itemDestroyed(item);
//FIXME: leaking incomlete type
    delete item;
    item = NULL;
}
