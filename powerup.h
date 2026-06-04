#ifndef POWERUP_H
#define POWERUP_H

#include <QPainter>
#include <QRect>

enum class PowerUpType
{
    Life
};

class PowerUp
{
public:
    PowerUp(int x, int y, PowerUpType type);

    void update();
    void draw(QPainter &painter) const;

    QRect rect() const;
    PowerUpType type() const;

    bool isOutOfScreen(int windowHeight) const;

private:
    int m_x;
    int m_y;
    int m_size;
    int m_speed;
    PowerUpType m_type;
};

#endif // POWERUP_H
