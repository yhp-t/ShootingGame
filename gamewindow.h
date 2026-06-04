#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QSet>

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

protected:
    // Qt 需要重绘时自动调用，我们在这里画游戏画面
    void paintEvent(QPaintEvent *event) override;
    // 键盘按下 / 松开
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();   // 每一帧执行一次（游戏的心跳）

private:
    QTimer *m_timer;            // 驱动游戏循环的定时器
    QSet<int> m_pressedKeys;    // 当前被按住的键（A2 的 Player 会用到，这个要保留）

    // ===== 以下是 A1 演示用占位，等队友 A2 做好 Player 后可删 =====
    int m_playerX;              // 占位方块横坐标
    int m_playerY;              // 占位方块纵坐标
    // ==========================================================

    // FPS 统计，用来确认循环稳定在 60 帧
    QElapsedTimer m_fpsTimer;
    int m_frameCount;
    int m_fps;
};

#endif // GAMEWINDOW_H
