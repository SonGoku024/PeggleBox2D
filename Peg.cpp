#include <QGraphicsPixmapItem>

#include "Peg.h"
#include "Game.h"
#include "Sprites.h"

using namespace PGG;

Peg::Peg()
{



}

b2Body* Peg::MasterPeg(QGraphicsScene* _world, b2World* world2d, bool cannonShot, QPoint midPos, QPoint currPos, bool nextFrame)
{
    if(!nextFrame)
    {
        // create master peg
        b2BodyDef ball;
        ball.type = b2_dynamicBody;
        ball.linearDamping = 0.1;
        ball.position.Set((_world->sceneRect().width() / 2) / 30.0, 0 / 30.0);
        _MasterPeg = world2d->CreateBody(&ball);

        b2CircleShape ballShape;
        ballShape.m_p.Set(0, 0);
        ballShape.m_radius = 0.2;

        b2FixtureDef ballFixtureDef;
        ballFixtureDef.restitution = 0.2;
        ballFixtureDef.shape = &ballShape;
        ballFixtureDef.density = 50.0f;
        _MasterPeg->CreateFixture(&ballFixtureDef);

        QGraphicsPixmapItem* ballItem;
        ballItem = new QGraphicsPixmapItem(0);
        ballItem->setPixmap(Sprites::instance()->get("player").scaled(14, 14));
        ballItem->setPos((_world->sceneRect().width() / 2), 0);
        _world->addItem(ballItem);
        _MasterPeg->SetUserData((ballItem));
    }
    else
    {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)_MasterPeg->GetUserData();
        item->setPos(_MasterPeg->GetPosition().x * 30.0, _MasterPeg->GetPosition().y * 30.0);

        if (_MasterPeg->GetPosition().y > 38)
        {
            _MasterPeg->SetTransform(b2Vec2((_world->sceneRect().width() / 2) / 30.0, 0 / 30.0), _MasterPeg->GetAngle());
            _MasterPeg->SetLinearVelocity(b2Vec2(0, 0));
            _MasterPeg->SetAngularVelocity(0);
            world2d->SetGravity(b2Vec2(0, 0));
        }
    }
    if (cannonShot)
    {
        _MasterPeg->SetLinearVelocity(b2Vec2((currPos.x()-midPos.x())/50, (currPos.y()-midPos.y())/50));
    }

    return _MasterPeg;
}

void Peg::CirclePeg(QGraphicsScene* _world, b2World* world2d,int x,int y, QString color)
{
    //create pegs
    b2Body* pegBall;

    b2BodyDef peg;
    QGraphicsPixmapItem* pegItem;

    peg.type = b2_staticBody;
    peg.linearDamping = 0.4;
    peg.position.Set(x/30.0, y/30.0);
    pegBall = world2d->CreateBody(&peg);

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 0.2;

    b2FixtureDef fixtureDef;
    fixtureDef.restitution = 1;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 13.0f;
    pegBall->CreateFixture(&fixtureDef);

    pegItem = new QGraphicsPixmapItem(0);
    pegItem->setPixmap(Sprites::instance()->get("peg_blue").scaled(18, 18));
    pegItem->setPos(x, y);
    _world->addItem(pegItem);
    pegBall->SetUserData(pegItem);

}

void Peg::setSprite()
{
    //blue

    //red

    //violet

    //blue hit

    //
}

void getMasterPeg()
{

}
