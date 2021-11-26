#pragma once

#include <QGraphicsScene>
#include <QPoint>
#include <QPainter>
#include <QVector>
#include "box2d/include/box2d/b2_settings.h"
#include "box2d/include/box2d/box2d.h"

namespace PGG
{
class Peg;
}

class PGG::Peg
{
private:

    b2Body* _MasterPeg;

    int remainingBall = 9;
    QGraphicsPixmapItem* remainingBallPixmap;

    //QVector<b2Body*> pegBall;

public:
    Peg();

    void CirclePeg(QGraphicsScene* _world, b2World* world2d,int x,int y, QString color);
    b2Body* MasterPeg(QGraphicsScene* _world, b2World* world2d, bool cannonShot, QPoint midPos, QPoint currPos, bool nextFrame=false);

    void getMasterPeg();
    void setSprite();

};
