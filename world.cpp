#include "world.h"

QBox2DWorld::QBox2DWorld(QObject* parent): QObject(parent),
    _contactsCount(0),
    _mouseJoint(NULL) {
    _world = new b2World(b2Vec2(0,0));
    _world->SetContactListener(this);
    b2BodyDef bd;
    _groundBody = _world->CreateBody(&bd);
}

void QBox2DWorld::step(){
    _world->Step(_timeStep,_velocityIterations,_positionIterations);
    _scene->advance();
}

QBox2DRectItem* QBox2DWorld::createBox(const QPointF& pos) {
    QBox2DRectItem *box = new QBox2DRectItem();
    box->setBodyType(b2_dynamicBody);
    uint l = 10;
    QRectF rect(0, 0, l, l);
    box->setShape(rect);
    box->setPos(pos.x()-l, pos.y()-l);
    box->setFriction(0.9f);
    box->setDensity(1.0f);
    box->setRestitution(0.5f);
    box->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    box->create(_world);
    _scene->addItem(box);
    return box;
}

void QBox2DWorld::grabItem(const QPointF &p) {

    if (_mouseJoint != NULL){
        return;
    }

    b2Vec2 pos;
    pos.x =  Q2W_(p.x());
    pos.y = -Q2W_(p.y());

    // Make a small box.
    b2AABB aabb;
    b2Vec2 d;
    d.Set(0.001f, 0.001f);
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

void TestWorld::create(QGraphicsScene* const scene){
    _scene = scene;
    _world->SetGravity(b2Vec2(0, -10));
    QBox2DRectItem *ground = new QBox2DRectItem();
    QRectF groundrect(0, 0, 400, 5);
    QColor groundColor(64, 64, 64);
    ground->setShape(groundrect);
    ground->setPos(-200, 0);
    ground->setBrush(groundColor);
    ground->setBodyType(b2_staticBody);
    ground->create(_world);
    _scene->addItem(ground);
    _box = createBox(QPointF(0, -10));
}

void ExampleWorld::create(QGraphicsScene* const scene){
    _scene = scene;
    _world->SetGravity(b2Vec2(0, -10));

    // ground
    QBox2DRectItem *ground = new QBox2DRectItem();
    QRectF groundrect(0, 0, 400, 5);
    QColor groundColor(64, 64, 64);
    ground->setShape(groundrect);
    ground->setPos(-200, 0);
    ground->setBrush(groundColor);
    ground->create(_world);
    scene->addItem(ground);

    // roof
    QBox2DRectItem *roof = new QBox2DRectItem();
    roof->setShape(groundrect);
    roof->setPos(-200, -405);
    roof->setBrush(groundColor);
    roof->create(_world);
    scene->addItem(roof);

    //left wall
    QBox2DRectItem *leftWall = new QBox2DRectItem();
    QRectF rect(0, 0, 5, 400);
    leftWall->setShape(rect);
    leftWall->setPos(-205, -400);
    leftWall->setBrush(groundColor);
    leftWall->create(_world);
    scene->addItem(leftWall);

    //right wall
    QBox2DRectItem *rightWall = new QBox2DRectItem();
    rightWall->setShape(rect);
    rightWall->setPos(200, -400);
    rightWall->setBrush(groundColor);
    rightWall->create(_world);
    scene->addItem(rightWall);

    //add motors
    for (int i = 0; i < 2; ++i) {
        QBox2DRectItem *staticBox = new QBox2DRectItem();
        staticBox->setShape(QRectF(0, 0, 10, 50));
        staticBox->setPos(pow(-1.0f,i) * 60, -50);
        staticBox->setRotation(0);
        staticBox->setBrush(groundColor);
        staticBox->setBodyType(b2_dynamicBody);
        staticBox->setFriction(1.0f);
        staticBox->setDensity(2.0f);
        staticBox->setRestitution(0.1f);
        staticBox->create(_world);
        scene->addItem(staticBox);
        b2RevoluteJointDef jd1;
        jd1.Initialize(staticBox->body(), ground->body(), staticBox->body()->GetPosition());
        jd1.motorSpeed = (float32)(pow(-1.0f,i) * 2 * b2_pi);
        jd1.maxMotorTorque = 50000000.0f;
        jd1.enableMotor = true;
        _world->CreateJoint(&jd1);
        //joints.append(revJoint);
    }

    //add falling boxes
    for (int i = 0; i < 10; ++i) {
        int size = 6 + qrand() % 10;
        QBox2DRectItem *item = new QBox2DRectItem();
        item->setShape(QRectF(0,0,size,size));
        item->setPos(-80 + qrand() % 40, -75 - qrand() % 150);
        item->setRotation(qrand() % 360);
        item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        item->setBodyType(b2_dynamicBody);
        item->setDensity(1.0f);
        item->setFriction(0.8f);
        item->setRestitution(0.4f);
        item->create(_world);
        scene->addItem(item);
    }

    // add triangles
    for (int i = 0; i < 10; ++i) {
        int size = 4 + qrand() % 7;
        QPolygonF polygonForm;
        polygonForm << QPointF(0, -size*2) << QPointF(-size, 0) << QPointF(size, 0);
        QBox2DPolygonItem *testItem = new QBox2DPolygonItem();
        testItem->setShape(polygonForm);
        testItem->setPos(-20 + qrand() % 60, -50 - qrand() % 150);
        testItem->setRotation(qrand() % 360);
        testItem->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        testItem->setFriction(1.0f);
        testItem->setDensity(5.0f);
        testItem->setRestitution(0.5f);
        testItem->setBodyType(b2_dynamicBody);
        testItem->create(_world);
        scene->addItem(testItem);
    }

    //Add circles
    for (int i = 0; i < 600; ++i) {
        int size = 3;
        QBox2DCircleItem *item = new QBox2DCircleItem();
        item->setShape(QRectF(-size , -size, size*2, size*2));
        item->setPos(0 + qrand() % 60, -100 - qrand() % 150);
        item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
        item->setBodyType(b2_dynamicBody);
        item->setFriction(0.1f);
        item->setDensity(1.0f);
        item->setRestitution(0.1f);
        item->create(_world);
        scene->addItem(item);
    }
}

void ArcanoidWorld::create(QGraphicsScene* const scene) {
        _scene = scene;
        scene->setSceneRect(-200, 0, 400, 400);
        _world->SetGravity(b2Vec2(0, -1));

        QBox2DRectItem* groundl = new QBox2DRectItem();
        groundl->setPos(-205, 0);
        groundl->setShape(QRectF(0, 0, 5, 400));
        groundl->setBrush(QColor(128, 128, 128));
        groundl->setRestitution(1.0f);
        groundl->create(_world);
        scene->addItem(groundl);

        QBox2DRectItem* groundr = new QBox2DRectItem();
        groundr->setPos(200, 0);
        groundr->setShape(QRectF(0, 0, 5, 400));
        groundr->setBrush(QColor(128, 128, 128));
        groundr->setRestitution(1.0f);
        groundr->create(_world);
        scene->addItem(groundr);

        QBox2DRectItem* groundup = new QBox2DRectItem();
        groundup->setPos(-200, 0);
        groundup->setShape(QRectF(0, 0, 400, 5));
        groundup->setBrush(QColor(128, 128, 128));
        groundup->create(_world);
        scene->addItem(groundup);

        QBox2DRectItem* hor_item = new QBox2DRectItem();
        hor_item->setPos(0, 400);
        hor_item->setShape(QRectF(0, 0, 5, 5));
        hor_item->setBrush(QColor(128, 128, 128));
        hor_item->setBodyType(b2_dynamicBody);
        hor_item->setDensity(0.0f);
        hor_item->create(_world);
        scene->addItem(hor_item);

        _paddle = new QBox2DRectItem();
        _paddle->setPos(-50, 400 - 5);
        _paddle->setShape(QRectF(0, 0, 100, 5));
        _paddle->setBrush(QColor(128, 128, 128));
        _paddle->setBodyType(b2_dynamicBody);

        _paddle->setFriction(1.0f);
        _paddle->setDensity(1.0f);
        _paddle->setRestitution(1.0f);
        _paddle->create(_world);
        //_paddle->body()->SetType(b2_staticBody);
        scene->addItem(_paddle);

        b2PrismaticJointDef hor_joint_def;
        b2Vec2 axis(1.0f, 0.0f);

        hor_joint_def.Initialize(hor_item->body(), groundl->body(), b2Vec2(0,0), axis);
        hor_joint_def.lowerTranslation = -2.96f;
        hor_joint_def.upperTranslation = 2.96f;
        hor_joint_def.enableLimit = true;
        _world->CreateJoint(&hor_joint_def);

        b2PrismaticJointDef vert_joint_def;
        axis = b2Vec2(0.0f, 1.0f);
        axis.Normalize();

        vert_joint_def.Initialize(_paddle->body(), hor_item->body(), b2Vec2(0,0), axis);
        vert_joint_def.lowerTranslation = -0.5f;
        vert_joint_def.upperTranslation = 0.0f;
        vert_joint_def.enableLimit = true;
        _world->CreateJoint(&vert_joint_def);


        for (int i = 0; i < 1; ++i) {
            int size = 5;
            QBox2DCircleItem *item = new QBox2DCircleItem();
            item->setShape(QRectF(-size , -size, size*2, size*2));
            item->setPos(0, 5);
            item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            item->setBodyType(b2_dynamicBody);
            item->setFriction(1.0f);
            item->setDensity(1.0f);
            item->setRestitution(1.0f);
            item->create(_world);
            item->body()->SetBullet(true);
            scene->addItem(item);
        }

    }


void ArcanoidWorld::updateKeys(int key, int state) {
        if( state == 1 ) {
            switch( key ) {
            case Qt::Key_W:
                _paddle->body()->ApplyLinearImpulse(b2Vec2(0.0, 1.0f),_paddle->body()->GetWorldCenter());
            break;
            case Qt::Key_S:
                _paddle->body()->ApplyLinearImpulse(b2Vec2(0.0, -1.0f),_paddle->body()->GetWorldCenter());
            break;
            case Qt::Key_A:
                _paddle->body()->ApplyForceToCenter(b2Vec2(-500.0f,0.0f));
            break;
            case Qt::Key_D:
                _paddle->body()->ApplyForceToCenter(b2Vec2(500.0f,0.0f));
            break;
            case Qt::Key_Q:
                _paddle->body()->ApplyTorque(1000);
            break;
            case Qt::Key_E:
                _paddle->body()->ApplyTorque(-1000);
            break;
            }
        }
        else {

        }
    }
