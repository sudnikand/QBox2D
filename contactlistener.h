#ifndef QBOX2DCONTACTLISTENER_H
#define QBOX2DCONTACTLISTENER_H

#include <Box2D.h>
#include <QList>
#include "items.h"

class ContactPoint;

class QBox2DContactListener: public b2ContactListener {

public:
    QBox2DContactListener();
    virtual ~QBox2DContactListener();

    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    QList<ContactPoint> _contacts;
};

struct ContactPoint {
    QBox2DItem* itemA;
    QBox2DItem* itemB;

    bool operator ==(const ContactPoint& other){
        if (this->itemA == other.itemA &&
            this->itemB == other.itemB) {
            return true;
        }
        return false;
    }
};

#endif // QBOX2DCONTACTLISTENER_H
