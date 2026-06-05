#include "powerup.h"

PowerUp::PowerUp(int x, int y, PowerUpType type)
{
    m_x = x;
    m_y = y;
    m_type = type;

    m_size = 28;
    m_speed = 3;
}

void PowerUp::update()
{
    m_y += m_speed;
}

void PowerUp::draw(QPainter &painter) const
{
    painter.setPen(Qt::NoPen);

    if (m_type == PowerUpType::Life) {
        painter.setBrush(QColor(255, 210, 80));   // 黄色：生命道具
    } else if (m_type == PowerUpType::WeaponUpgrade) {
        painter.setBrush(QColor(160, 120, 255));  // 紫色：武器升级道具
    }

    painter.drawEllipse(rect());

    painter.setPen(Qt::black);

    if (m_type == PowerUpType::Life) {
        painter.drawText(rect(), Qt::AlignCenter, "+");
    } else if (m_type == PowerUpType::WeaponUpgrade) {
        painter.drawText(rect(), Qt::AlignCenter, "W");
    }
}


QRect PowerUp::rect() const
{
    return QRect(
        m_x - m_size / 2,
        m_y - m_size / 2,
        m_size,
        m_size
    );
}

PowerUpType PowerUp::type() const
{
    return m_type;
}

bool PowerUp::isOutOfScreen(int windowHeight) const
{
    return m_y - m_size / 2 > windowHeight;
}
