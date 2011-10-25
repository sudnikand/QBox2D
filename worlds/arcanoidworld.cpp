#include "arcanoidworld.h"

ArcanoidWorld::ArcanoidWorld() : QBox2DWorld() {
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
