#include "arcanoidworld.h"
#include "brick.h"

ArcanoidWorld::ArcanoidWorld(QObject *parent) : QBox2DWorld(parent) {
    _contactListener = new QBox2DContactListener();
    _world->SetContactListener(_contactListener);
    qDebug()<< "Arcanoid world created";
}

ArcanoidWorld::~ArcanoidWorld() {
    delete _contactListener;
    _contactListener = NULL;
    qDebug() << "Deleting arcanoid world";
}

void ArcanoidWorld::step(){
    QBox2DWorld::step();

    if (!_ball || !_paddle || !_bound) return;

    adjustBallSpeed();

    QList<ContactPoint> &_contacts = _contactListener->_contacts;

    if (_contacts.size() == 0) {
        return;
    }

    QSet<QBox2DItem*> destroyItems;

    for(int i = 0; i < _contacts.size() ; ++i){
        ContactPoint cp = _contacts.at(i);
        QBox2DItem *item = ( cp.itemA == _ball ) ? cp.itemB :
                           ( cp.itemB == _ball ) ? cp.itemA : NULL;

        if (!item) continue;

        emit hit();

        if (item == _paddle) continue;
        if (item == _bound) {
            float32 r = _ball->body()->GetFixtureList()->GetShape()->m_radius;
            destroyItem(_ball);
            createBall(r);
            _paddle->body()->SetTransform((b2Vec2(WSCALE2(0, 15))),0);
                       // Add game over here
        }

        if (item->handleContact() < 0) {
            destroyItems.insert(item);
        }
    }

    QSetIterator<QBox2DItem*> i(destroyItems);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        destroyItem(item);
    }

    if (!findItem("brick")) {
        emit gameFinished();
    }

}

void ArcanoidWorld::populate() {
    qDebug()<< "Starting Arcanoid World";

    loadWorld("data/levels/arcanoid_01.xml");
    _world->SetGravity(b2Vec2(0,0));

    float32 fieldSize = 40;
    QColor gray = QColor(128, 128, 128);
    b2PolygonShape shape;

    _bound = findItem("bound"); if (!_bound) return;

    _ball = findItem("ball"); if (!_ball) return;
    _ball->body()->SetBullet(true);
    _paddle = findItem("paddle");

    QBox2DItem* horItem = new QBox2DItem();
    horItem->setPos(b2Vec2(WSCALE2(0, fieldSize)));
    horItem->setBodyType(b2_dynamicBody);
    horItem->setDensity(0.0f);
    horItem->createBody(_world);
    shape.SetAsBox(WSCALE2(1,1));
    horItem->setShape(shape);
    horItem->setColor(gray);
    appendItem(horItem);

    b2PrismaticJointDef horJointDef;
    b2Vec2 axis(1.0f, 0.0f);

    horJointDef.Initialize(horItem->body(), _ground, b2Vec2(0,0), axis);
    horJointDef.lowerTranslation = WSCALE(-fieldSize);
    horJointDef.upperTranslation = WSCALE(fieldSize);
    horJointDef.enableLimit = true;
    _world->CreateJoint(&horJointDef);

    b2PrismaticJointDef vertJointDef;
    axis = b2Vec2(0.0f, 1.0f);

    vertJointDef.Initialize(_paddle->body(), horItem->body(), b2Vec2(0,0), axis);
    vertJointDef.lowerTranslation = 0.0f;
    vertJointDef.upperTranslation = WSCALE(fieldSize/20);
    vertJointDef.enableLimit = true;
    _world->CreateJoint(&vertJointDef);

    int n = 10;
    float32 brickWidth = 1;
    int xStep = fieldSize/n;
    int yStep = fieldSize/n;
    for (int j = 0; j < n-1; ++j){
        for (int i = 0; i < n; ++i){
            Brick* brick = new Brick();
            int xPos = i * xStep - fieldSize/2 + brickWidth*2;
            int yPos = j * yStep - fieldSize/2 + brickWidth*2;
            brick->setPos(b2Vec2(WSCALE2(xPos,yPos)));
            brick->setRestitution(0.1f);
            brick->setFriction(0);
            brick->setDensity(10.0f);
            brick->setBodyType(b2_dynamicBody);
            brick->createBody(_world);
            b2PolygonShape brickShape;
            brickShape.SetAsBox(WSCALE2(brickWidth,brickWidth/2));
            brick->setShape(brickShape);
            //brick->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            brick->setColor(Qt::white);
            brick->body()->SetUserData(brick);
            brick->setName("brick");
            brick->setDurability(qrand() % 3);
            appendItem(brick);

            b2RevoluteJointDef jointDef;
            jointDef.Initialize(brick->body(), _ground, brick->body()->GetWorldCenter());
            jointDef.enableMotor = true;
            jointDef.motorSpeed = pow(-1.0f,i+j) * (qrand() % 100) / 10;
            jointDef.maxMotorTorque = 5000.0f;
            _world->CreateJoint(&jointDef);
        }
    }

}


void ArcanoidWorld::handleKeyPressed(const int &key)
{
    switch( key ) {
    case Qt::Key_W:
        _paddle->body()->ApplyLinearImpulse(b2Vec2(WSCALE2(0.0, -10.0f)),_paddle->body()->GetWorldCenter(), true);
        break;
    case Qt::Key_S:
        _paddle->body()->ApplyLinearImpulse(b2Vec2(WSCALE2(0.0, 10.0f)),_paddle->body()->GetWorldCenter(), true);
        break;
    case Qt::Key_A:
        _paddle->body()->ApplyLinearImpulse(b2Vec2(WSCALE2(-10.0f,0.0f)),_paddle->body()->GetWorldCenter(), true);
        break;
    case Qt::Key_D:
        _paddle->body()->ApplyLinearImpulse(b2Vec2(WSCALE2(10.0f,0.0f)),_paddle->body()->GetWorldCenter(), true);
        break;
    case Qt::Key_Q:
        _paddle->body()->ApplyTorque(1000,false);
        break;
    case Qt::Key_E:
        _paddle->body()->ApplyTorque(-1000, false);
        break;
    }
}

void ArcanoidWorld::createBall(float32 radius){
    QBox2DItem *ball = new QBox2DItem();
    ball->setBodyType(b2_dynamicBody);
    ball->setPos(b2Vec2(WSCALE2(0, 0)));
    ball->setFriction(0.5f);
    ball->setDensity(1.0f);
    ball->setRestitution(1.1f);
    ball->createBody(_world);

    b2CircleShape circle;
    circle.m_radius = radius;
    ball->setShape(circle);
    //ball->setColor(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
    ball->setColor(Qt::white);
    ball->body()->SetUserData(ball);
    ball->body()->SetBullet(true);
    ball->setTextureName("ball.png");
    ball->setName("ball");
    appendItem(ball);
    _ball = ball;
    //_ball->body()->ApplyLinearImpulse(b2Vec2(0,30), _ball->body()->GetWorldCenter(),true);
}

void ArcanoidWorld::adjustBallSpeed() {
    b2Vec2 velocity = _ball->body()->GetLinearVelocity();
    float32 speed = velocity.Length();
    float32 maxSpeed = WSCALE(20);

     if (speed > maxSpeed) {
         //qDebug() << "Damping speed: " << speed;
         _ball->body()->SetLinearDamping(0.4);
     } else if (speed <= maxSpeed) {
         _ball->body()->SetLinearDamping(0.0);
         if (speed < WSCALE(1)){
             qDebug() << "Speed up: " << speed;
            _ball->body()->ApplyLinearImpulse(b2Vec2(WSCALE2(0,-10)), _ball->body()->GetWorldCenter(),true);
         }
     }
}
