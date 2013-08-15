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

void QBox2DWorld::parseXML(const QDomElement &root){

    {
        QDomElement gravity = root.firstChildElement("gravity");
        if(!gravity.isNull() && gravity.hasAttribute("strength")){
            _world->SetGravity(b2Vec2(gravity.attribute("direction").toFloat(),
                                      gravity.attribute("strength").toFloat()));
        }
    }

    { //Parsing objects
        QDomElement objects = root.firstChildElement("objects");
        if ( objects.isNull() ) {
            return;
        }

        QDomElement object = objects.firstChildElement( "object" );
        while (!object.isNull()) {
            QBox2DItem *item = new QBox2DItem();

            if(object.attribute("bodyType") == "dynamic" ){
                item->setBodyType(b2_dynamicBody);
            }

            if(object.hasAttribute("name")){
                item->setName(object.attribute("name"));
            }

            {
                QDomElement position = object.firstChildElement("position");
                if (!position.isNull()){
                    item->setPos(b2Vec2(WSCALE2(position.attribute("x").toFloat(),
                                        position.attribute("y").toFloat())
                                        ));
                    if (position.hasAttribute("rotation"))
                        item->setRotation(position.attribute("rotation").toFloat());
                }
            }

            {
                QDomElement physic = object.firstChildElement("physic");
                if (!physic.isNull()){
                    item->setDensity(physic.attribute("density").toFloat());
                    item->setFriction(physic.attribute("friction").toFloat());
                    item->setRestitution(physic.attribute("restitution").toFloat());
                }
            }

            item->createBody(_world);
            item->body()->SetUserData(item);

            {
                QDomElement geometry = object.firstChildElement("geometry");
                if (geometry.attribute("type") == "box"){
                    b2PolygonShape shape;
                    shape.SetAsBox(WSCALE2(geometry.attribute("width").toFloat()/2,
                                   geometry.attribute("height").toFloat()/2));
                    item->setShape(shape);
                } else if (geometry.attribute("type") == "circle"){
                    b2CircleShape circle;
                    circle.m_radius = geometry.attribute("radius").toFloat();
                    item->setShape(circle);
                }
            }

            {
                QDomElement color = object.firstChildElement("color");
                if (!color.isNull()){
                    item->setColor(QColor(color.text()));
                }
            }

            {
                QDomElement texture = object.firstChildElement("texture");
                if (!texture.isNull()){
                    item->_textureName = texture.text();
                }
            }

            appendItem(item);
            object = object.nextSiblingElement( "object" );
        }
    }

    { //Parsing joints
        QDomElement joints = root.firstChildElement("joints");
        if ( joints.isNull() ) {
            return;
        }

        QDomElement jointNode = joints.firstChildElement( "joint" );
        while (!jointNode.isNull()) {
            if(jointNode.attribute("type") == "revolute" ){
                b2RevoluteJointDef jointDef;
                QDomElement bodiesNode = jointNode.firstChildElement("bodies");
                if(!bodiesNode.isNull()){
                    b2Body* bodyA = findItem( bodiesNode.attribute("a"))->body();
                    b2Body* bodyB = NULL;
                    if (bodiesNode.attribute("b") == "_ground" ){
                        bodyB = _ground;
                    } else {
                        bodyB = findItem( bodiesNode.attribute("b"))->body();
                    }
                    if( bodyA && bodyB ) {
                        jointDef.Initialize(bodyA, bodyB, bodyA->GetPosition());
                    }
                }

                QDomElement motorNode = jointNode.firstChildElement("motor");
                if(!motorNode.isNull()){
                    jointDef.motorSpeed = motorNode.attribute("speed").toFloat();
                    jointDef.maxMotorTorque = motorNode.attribute("torque").toFloat();
                    jointDef.enableMotor = motorNode.attribute("enable") == "true";
                }
                _world->CreateJoint(&jointDef);
            }
            jointNode = jointNode.nextSiblingElement( "joint" );
        }
    }
}

void QBox2DWorld::loadWorld(const QString &filename){
    qDebug() << "In loadworld";
    QDomDocument domDoc("world");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "XML file not found";
        return;
    }
    if (!domDoc.setContent(&file)) {
        qDebug() << "Cannot set file content";
        file.close();
        return;
    }
    file.close();
    qDebug() << "Reading XML file";

    QDomElement root = domDoc.documentElement();
    if (root.tagName() != "world") {
        qDebug() << "Not a world file";
        return;
    }

    parseXML(root);

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


    for(b2Body *body = _world->GetBodyList(); body; body = body->GetNext()) {
        if (body->GetUserData() != NULL) {
            QBox2DItem *item = static_cast<QBox2DItem*>(body->GetUserData());
            item->update();
        }
    }

    _world->Step(_timeStep,_velocityIterations,_positionIterations);
}

QBox2DItem* QBox2DWorld::createBox(const QPointF& pos) {
// position is in world coordinates
    QBox2DItem *box = new QBox2DItem();
    box->setBodyType(b2_dynamicBody);
    box->setFriction(0.9f);
    box->setDensity(1.0f);
    box->setRestitution(0.5f);
    box->setPos(b2Vec2(pos.x(), pos.y()));
    box->createBody(_world);

    float32 l = WSCALE(10.0f);
    b2PolygonShape rect;
    rect.SetAsBox(l/2,l/2);
    box->setShape(rect);
    box->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    box->_textureName = "kde.png";
    box->body()->SetUserData(box);
    appendItem(box);
    qDebug() << "Box created";
    return box;
}

void QBox2DWorld::grabItem(const QPointF &p) {
// grab item in world coordinate p
    if (_mouseJoint != NULL){
        return;
    }

    b2Vec2 pos( p.x(), p.y() );

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
// move item to world coordinate
    if(_mouseJoint){
        _mouseJoint->SetTarget(b2Vec2( p.x(), p.y() ));
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
    for (b2JointEdge* edge = item->body()->GetJointList(); edge; edge = edge->next) {
        if (edge->joint == _mouseJoint)
            _mouseJoint = NULL;
    }

    _world->DestroyBody(item->body());
    removeItem(item);
    delete item;
    item = NULL;
}

void QBox2DWorld::appendItem(QBox2DItem *item){
    emit itemCreated(item);
}

void QBox2DWorld::removeItem(QBox2DItem *item){
    emit itemDestroyed(item);
}

QBox2DItem* QBox2DWorld::findItem(const QString &itemName){
    for(b2Body *body = _world->GetBodyList(); body; body = body->GetNext()) {
        if (body->GetUserData() != NULL) {
            QBox2DItem *item = static_cast<QBox2DItem*>(body->GetUserData());
            if (item->name() == itemName){
                return item;
            }
        }
    }
    return NULL;
}
