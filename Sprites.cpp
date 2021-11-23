#include "Sprites.h"
#include <QBitmap>
#include <iostream>
#include <QPainter>

using namespace PGG;

// load texture with transparency using the given color as mask
static QPixmap loadTexture(const std::string &file, QColor mask_color = Qt::magenta)
{
    QPixmap pixmap(file.c_str());
    pixmap.setMask(pixmap.createMaskFromColor(mask_color));
    return pixmap;
}

// moving within sprite utility function //non ci serve
QRect moveBy(QRect rect, int x, int y, int dx = 16, int dy = 16, int border_x = 1, int border_y = 1)
{
    rect.moveTo(QPoint(rect.x() + x * dx + x * border_x, rect.y() + y * dy + y * border_y));
    return rect;
}

// main object positions within sprites //non ha senso da togliere perché non ci serve
static QRect player(467, 259, 93, 93);
static QRect pegle(0, 0, 200, 200);
static QRect bucket(168, 200, 168, 24);

Sprites *Sprites::instance()
{
    static Sprites uniqueInstance;
    return &uniqueInstance;
}

Sprites::Sprites() //load in RAM
{
    title_screen = loadTexture(":/sprites/peggle_title.png", QColor(255, 0, 255));

    Hud_Unicorn = loadTexture(":/sprites/krita.png"); // da fare prob. nell'altra classe Hud
    //Hud_Drake ...

    //dynamic Hud da fare

    stage_elements = loadTexture(":/sprites/stage_elements.png", QColor(255, 0, 255));
    pegles = loadTexture(":/sprites/peggles.png", QColor(255, 255, 255));
}

QPixmap Sprites::get(const std::string &id)
{
    if (id == "peggle_title")
        return title_screen;

    else if (id == "Hud_Unicorn")
        return Hud_Unicorn;

    else if (id == "player")
        return loadTexture(":/sprites/ball.png", QColor(0, 0, 0)).copy(QRect(0, 0, 100, 100));
    else if (id == "bucket")
     return stage_elements.copy(bucket);
    // else if (id == "cannon")
    //    return stage_elements.copy(cannon);

    else if (id == "peg_blue_hit")
        return pegles.copy(pegle);
    else if (id == "peg_red_hit")
        return pegles.copy(moveBy(pegle, 1, 0, 200));
    else if (id == "peg_blue")
        return pegles.copy(moveBy(pegle, 0, 1, 0, 200));
    else if (id == "peg_red")
        return pegles.copy(moveBy(pegle, 1, 1, 200, 200));

    else
    {
        std::cerr << "Cannot find sprite texture with id \"" << id << "\"\n";
        return QPixmap();
    }
}

//QPixmap Sprites::getScore(int s)
//{
//}
