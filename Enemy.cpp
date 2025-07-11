#include "Enemy.h"
#include <QScreen>
#include <QDebug>
#include <QRandomGenerator>
#include <Controller.h>

Enemy::Enemy(QObject* parent): m_x_speed(4), m_y_speed(2) {
    connect(&timer_mover, &QTimer::timeout, this, &Enemy::tick);
    timer_mover.start(20); // 20 milliseconds
    m_index = rand() % 5;


    QScreen* screen;
    screen = QGuiApplication::primaryScreen();
    m_min_x = screen->geometry().width() * 0.15f;
    m_max_x = screen->geometry().width() * 0.85;
    int rand_x = QRandomGenerator::global()->bounded(m_min_x, m_max_x + 1);
    setX(rand_x);

    m_max_y = (screen->geometry().height() / 100) * 110;
    setY(screen->geometry().height() * 0.05);
}

void Enemy::tick(){
    if(Controller::is_playing == false) return;
    setY(m_y + m_y_speed);
    if(m_y > m_max_y) {
        setY(m_max_y);
    }
}
