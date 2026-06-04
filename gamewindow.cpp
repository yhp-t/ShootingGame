#include "gamewindow.h"

#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include <QRandomGenerator>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Shooting Game");
    setFixedSize(480, 800);

    m_state = GameState::Playing;

    // 占位方块初始位置：底部中间
    m_playerX = 480 / 2;
    m_playerY = 800 - 80;

    // FPS 统计初始化
    m_frameCount = 0;
    m_fps = 0;
    m_playerHp = 3;
    m_fpsTimer.start();

    // 创建定时器：约每 16 毫秒跳一次 → 1000 / 16 ≈ 每秒 60 帧
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GameWindow::gameLoop);
    m_timer->start(16);
}

GameWindow::~GameWindow()
{
}

// 游戏心跳：每一帧调用一次
void GameWindow::gameLoop()
{

    if (m_state != GameState::Playing) {
        update();
        return;
    }

    // --- 1. 更新游戏状态 ---
    // 根据按住的方向键移动占位方块（A2 会把这段换成真正的 Player 逻辑）
    const int speed = 5;
    if (m_pressedKeys.contains(Qt::Key_Left))  m_playerX -= speed;
    if (m_pressedKeys.contains(Qt::Key_Right)) m_playerX += speed;
    if (m_pressedKeys.contains(Qt::Key_Up))    m_playerY -= speed;
    if (m_pressedKeys.contains(Qt::Key_Down))  m_playerY += speed;

    // 不让方块跑出窗口
    if (m_playerX < 20) m_playerX = 20;
    if (m_playerX > width() - 20) m_playerX = width() - 20;
    if (m_playerY < 20) m_playerY = 20;
    if (m_playerY > height() - 20) m_playerY = height() - 20;

    for (int i = 0; i < m_powerUps.size(); ++i) {
    m_powerUps[i].update();
}

    for (int i = m_powerUps.size() - 1; i >= 0; --i) {
        if (m_powerUps[i].isOutOfScreen(height())) {
            m_powerUps.removeAt(i);
    }
}

checkPowerUpPickup();


    // --- 2. 统计 FPS ---
    m_frameCount++;
    if (m_fpsTimer.elapsed() >= 1000) {   // 每过 1 秒算一次
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer.restart();
    }

    // --- 3. 请求重绘 → 触发 paintEvent ---
    update();
}

// 所有画面绘制都在这里
void GameWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景填成深色
    painter.fillRect(rect(), QColor(40, 44, 60));

    // 画占位玩家方块（A2 会用真正的载具替换）
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(90, 200, 250));
    painter.drawRect(m_playerX - 20, m_playerY - 20, 40, 40);

    for (const PowerUp &powerUp : m_powerUps) {
    powerUp.draw(painter);
}


    // 左上角显示 FPS，确认循环稳定在 60 帧
    painter.setPen(Qt::white);
    painter.drawText(10, 25, QString("FPS: %1").arg(m_fps));
    painter.drawText(10, 50, QString("HP: %1").arg(m_playerHp));

    if (m_state == GameState::Paused) {
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(24);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
}

}

// 键盘按下
void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;   // 忽略长按时系统的重复触发

    m_pressedKeys.insert(event->key());  // 记下这个键被按住了

    if (event->key() == Qt::Key_Space) {
        qDebug() << "Space pressed - Fire!";   // A1 验收点：控制台能看到输出
    }

    if (event->key() == Qt::Key_P) {
        spawnTestPowerUp();
        qDebug() << "Test power up spawned";
}

    if (event->key() == Qt::Key_O) {
        tryDropPowerUp(width() / 2, 80);
        qDebug() << "Simulate enemy died";
}


    if (event->key() == Qt::Key_Escape) {
    if (m_state == GameState::Playing) {
        m_state = GameState::Paused;
        qDebug() << "Game Paused";
    } else if (m_state == GameState::Paused) {
        m_state = GameState::Playing;
        qDebug() << "Game Resumed";
    }
}

}

// 键盘松开
void GameWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    m_pressedKeys.remove(event->key());  // 这个键松开了
}

QRect GameWindow::playerRect() const
{
    return QRect(m_playerX - 20, m_playerY - 20, 40, 40);
}

void GameWindow::spawnTestPowerUp()
{
    int x = width() / 2;
    int y = 60;

    m_powerUps.append(PowerUp(x, y, PowerUpType::Life));
}

void GameWindow::tryDropPowerUp(int x, int y)
{
    int chance = QRandomGenerator::global()->bounded(100);

    if (chance < 30) {
        m_powerUps.append(PowerUp(x, y, PowerUpType::Life));
        qDebug() << "Power up dropped at:" << x << y;
    } else {
        qDebug() << "No power up dropped";
    }
}


void GameWindow::checkPowerUpPickup()
{
    QRect player = playerRect();

    for (int i = m_powerUps.size() - 1; i >= 0; --i) {
        if (player.intersects(m_powerUps[i].rect())) {
            if (m_powerUps[i].type() == PowerUpType::Life) {
                m_playerHp += 1;
                qDebug() << "Picked up life power up. HP:" << m_playerHp;
            }

            m_powerUps.removeAt(i);
        }
    }
}
