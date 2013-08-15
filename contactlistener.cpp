#include "contactlistener.h"

QBox2DContactListener::QBox2DContactListener()
{
}

QBox2DContactListener::~QBox2DContactListener()
{
}

void QBox2DContactListener::BeginContact(b2Contact* contact) {
    ContactPoint cp;
    cp.itemA = static_cast<QBox2DItem*>(contact->GetFixtureA()->GetBody()->GetUserData());
    cp.itemB = static_cast<QBox2DItem*>(contact->GetFixtureB()->GetBody()->GetUserData());
    _contacts.append(cp);
}

void QBox2DContactListener::EndContact(b2Contact* contact) {
    ContactPoint cp;
    cp.itemA = static_cast<QBox2DItem*>(contact->GetFixtureA()->GetBody()->GetUserData());
    cp.itemB = static_cast<QBox2DItem*>(contact->GetFixtureB()->GetBody()->GetUserData());
    _contacts.removeOne(cp);
}

void QBox2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}

void QBox2DContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}
