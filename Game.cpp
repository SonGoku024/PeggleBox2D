#include <QApplication>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QIcon>
#include <cmath>

#include "Game.h"
#include "Peg.h"
#include "Bucket.h"

#include "Sounds.h"
#include "Sprites.h"

#include "box2d/include/box2d/b2_settings.h"
#include "box2d/include/box2d/box2d.h"

using namespace PGG;

// singleton
Game* Game::_uniqueInstance = 0;
Game* Game::instance()
{
    if (_uniqueInstance == 0)
        _uniqueInstance = new Game();
    return _uniqueInstance;
}

Game::Game() : QGraphicsView()
{
    // setup world/scene
    _world = new QGraphicsScene();
    _world->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    setScene(_world);

    setInteractive(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // setup game engine
    QObject::connect(&_engine, SIGNAL(timeout()),
                     this, SLOT(nextFrame()));
    _engine.setTimerType(Qt::PreciseTimer);
    _engine.setInterval(1000 / GAME_FPS);

    setWindowIcon(QIcon(":/icons/peggle.ico"));
    setWindowTitle("Peggle");

    reset();
    init();
}

void Game::reset()
{
    _engine.stop();
    _world->clear();

}

void Game::init()
{
    //video
    //suono
    //altro

    _world->addPixmap(QPixmap(Sprites::instance()->get("peggle_title")));
}

void Game::menuDuel()
{
    _world->clear();
    _state = GameState::MENU_DUEL;

    _world->addPixmap(QPixmap(Sprites::instance()->get("peggle_title")));

    //dopo aver scelto tutti i parametri del duello
    //click sulla apposito bottone per andare in PLAYING
}

void Game::buildLevel()
{
    _world->clear();

    //QGraphicsPixmapItem* level = _world->addPixmap(QPixmap(Sprites::instance()->get("Hud_Unicorn")));
    _world->addPixmap(QPixmap(Sprites::instance()->get("Hud_Unicorn")));

    //create physics world
    b2Vec2 gravity(0.0f, 5.0f);
    world2d = new b2World(gravity);
    world2d->SetAllowSleeping(false);
    //

    //create pegs
    pegBall.resize(27);
    QGraphicsPixmapItem* pegItem;
    b2BodyDef peg;
    int j = 0;
    int k = 0;

    for (int i = 0; i <= 26; i++)
    {
        if (k == 9)
        {
            k = 0;
            j++;
        }
        peg.type = b2_staticBody;
        peg.linearDamping = 0.4;
        peg.position.Set((1220 + (100 * k))/2 / 30.0, (300 + (100 * j)) / 30.0);
        pegBall[i] = world2d->CreateBody(&peg);

        pegItem = new QGraphicsPixmapItem(0);
        pegItem->setPixmap(Sprites::instance()->get("peg_blue").scaled(18, 18));
        pegItem->setPos((1220 + (100 * k))/2, (300 + (100 * j)));
        _world->addItem(pegItem);
        pegBall[i]->SetUserData(pegItem);

        b2CircleShape ballShape;
        ballShape.m_p.Set(0, 0);
        ballShape.m_radius = 0.2;

        b2FixtureDef ballFixtureDef;
        ballFixtureDef.restitution = 1;
        ballFixtureDef.shape = &ballShape;
        ballFixtureDef.density = 13.0f;
        pegBall[i]->CreateFixture(&ballFixtureDef);

        k++;
    }
    //

    // create master peg
    QGraphicsPixmapItem* ballItem;
    b2BodyDef ball;

    ball.type = b2_dynamicBody;
    ball.linearDamping = 0.1;
    ball.position.Set((sceneRect().width() / 2) / 30.0, 0 / 30.0);
    MasterPeg = world2d->CreateBody(&ball);
    ballItem = new QGraphicsPixmapItem(0);
    ballItem->setPixmap(Sprites::instance()->get("player").scaled(14, 14));
    ballItem->setPos((sceneRect().width() / 2), 0);
    _world->addItem(ballItem);
    MasterPeg->SetUserData((ballItem));

    b2CircleShape ballShape;
    ballShape.m_p.Set(0, 0);
    ballShape.m_radius = 0.2;

    b2FixtureDef ballFixtureDef;
    ballFixtureDef.restitution = 0.2;
    ballFixtureDef.shape = &ballShape;
    ballFixtureDef.density = 50.0f;

    MasterPeg->CreateFixture(&ballFixtureDef);
    //

    // CREATE PLATFORM------------------------------------------
    /*
        QGraphicsPolygonItem* groundItem;
        b2BodyDef ground;
        b2Body* realGround;
        ground.type = b2_staticBody;
        ground.linearDamping = 0.1;
        ground.position.Set(700 / 30.0, 600 / 30.0);
        realGround = world2d->CreateBody(&ground);
        groundItem = new QGraphicsPolygonItem(QPolygonF(QRectF(0, 0, 100, 10)));
        groundItem->setBrush(Qt::red);
        groundItem->setPos(700, 600);

        realGround->SetUserData(groundItem);
        _world->addItem(groundItem);
        //realGround->SetTransform(realGround->GetPosition(), -0.785398);
        b2PolygonShape groundShape;

        groundShape.m_radius = 0.3;
        groundShape.SetAsBox(100 / 2 / 30.0, 10 / 2 / 30.0 / 2);
        //groundItem->setRotation(-45);


        groundItem->setPos(QPointF(realGround->GetPosition().x * 30.0, realGround->GetPosition().y * 30.0));

        b2FixtureDef groundFixture;
        groundFixture.restitution = 1;
        groundFixture.shape = &groundShape;
        groundFixture.density = 13.0f;
        realGround->CreateFixture(&groundFixture);

        */

}

void Game::play() //in gioco
{
    showFullScreen();

    buildLevel();
    _engine.start();

    //ENGINE START

    _state = GameState::PLAYING;
}

void Game::nextFrame()
{
    world2d->Step(timeStep, velocityIterations, positionIterations); //sarebbe l'advance

    for (b2ContactEdge* edge = MasterPeg->GetContactList(); edge; edge = edge->next)
    {
        static_cast<QGraphicsPixmapItem*>(edge->contact->GetFixtureA()->GetBody()->GetUserData())->setPixmap(Sprites::instance()->get("peg_blue_hit").scaled(18, 18));
    }

    QGraphicsPixmapItem* item = (QGraphicsPixmapItem*)MasterPeg->GetUserData();
    item->setPos(MasterPeg->GetPosition().x * 30.0, MasterPeg->GetPosition().y * 30.0);

    if (MasterPeg->GetPosition().y > 25)
    {
        MasterPeg->SetTransform(b2Vec2((sceneRect().width() / 2) / 30.0, 0 / 30.0), MasterPeg->GetAngle());
        MasterPeg->SetLinearVelocity(b2Vec2(0, 0));
        MasterPeg->SetAngularVelocity(0);
        world2d->SetGravity(b2Vec2(0, 0));
    }

}

// EVENTI
void Game::mousePressEvent(QMouseEvent* e)
{
    if (_state != GameState::PLAYING && _state != GameState::PAUSED &&_state != GameState::MENU_DUEL)
    {
        menuDuel();
        return;
    }

    if (_state == GameState::MENU_DUEL)
    {
        play();
        return;
    }

    if (e->button()==Qt::LeftButton && _state==GameState::PLAYING)
    {
        double angle = atan2(MasterPeg->GetPosition().y - QCursor::pos().y(), MasterPeg->GetPosition().x - QCursor::pos().x());
        MasterPeg->SetLinearVelocity(b2Vec2(-cos(angle) * 10, -sin(angle) * 10));
    }

    if (e->button() == Qt::RightButton)
    {
        _engine.setInterval(5);
        world2d->SetGravity(b2Vec2(0, 5.0f));
    }

}

void Game::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
        _engine.setInterval(1000 / GAME_FPS);
}

void Game::mouseMoveEvent(QMouseEvent* e)
{
    setMouseTracking(true);
    QPoint midPos((sceneRect().width() / 2), 0), currPos;


    QGraphicsLineItem* item;
    item = new QGraphicsLineItem();
    item->setPen(QPen(Qt::red));

    currPos = e->pos();
    item->setLine(QLineF(midPos, currPos));
    _world->addItem(item);

    //    QPoint screenMiddle((sceneRect().width() / 2), 0);

    //    QPainter painter(this);
    //    QPen pen(Qt::red,10);
    //    //p.setPen(pen);
    //    painter.setPen(pen);


    //    painter.drawLine(screenMiddle.x(), screenMiddle.y(), e->pos().x(), e->pos().y());

}

void Game::wheelEvent(QWheelEvent* e)
{
    if (e->angleDelta().y() > 0)
        scale(1.1, 1.1);
    else
        scale(1 / 1.1, 1 / 1.1);
}

void Game::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_S && _state == GameState::MENU_DUEL)
    {
        play();
    }
    if (e->key() == Qt::Key_P && _state == GameState::PLAYING)
    {
        togglePause();
    }
    if (e->key() == Qt::Key_R && _state == GameState::PLAYING)
    {
        reset();
        menuDuel();
    }
    if (e->key() == Qt::Key_C)
    {
        toggleColliders();
    }
}

void Game::resizeEvent(QResizeEvent* e)
{

    fitInView(0, 0, sceneRect().width(), sceneRect().height(), Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(e);

}

// UTILITY
void Game::togglePause()
{
    if (_state == GameState::PLAYING)
    {
        _engine.stop();
        _state = GameState::PAUSED;
    }
    else if (_state == GameState::PAUSED)
    {
        _engine.start();
        _state = GameState::PLAYING;
    }
}

void Game::toggleColliders()
{
    //    for (auto item : _world->items())

    //        if (dynamic_cast<Object*>(item))
    //            dynamic_cast<Object*>(item)->toogleColliderVisible();
}

void Game::updateFPS()
{
    _FPS_label->setText(QString("FPS = ") + QString::number(_frame_count));
    _frame_count = 0;

    /*// setup FPS display and measuring
    _FPS_label = new QLabel("FPS =           ", this);
    _FPS_label->setFont(QFont("Consolas", 10));
    _FPS_label->move(QPoint(40, 5));
    QObject::connect(&_FPS_timer, SIGNAL(timeout()), this, SLOT(updateFPS()));
    _FPS_timer.setTimerType(Qt::PreciseTimer);
    _FPS_timer.setInterval(1000);
    _FPS_timer.start();*/
}
