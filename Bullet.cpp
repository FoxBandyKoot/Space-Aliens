#include "Bullet.h"
#include "Controller.h"

Bullet::Bullet(Controller* controller, QObject* parent):m_x_speed(14), m_y_speed(14), m_min_x(0), m_min_y(-50) {
    connect(&timer, &QTimer::timeout, this, &Bullet::updateBullet);
    timer.start(16); // 16 milliseconds, apply 60 times per seconds

    connect(this, &Bullet::bullet_destroy, controller, &Controller::delete_bullet);

}

void Bullet::updateBullet(){
    setY(m_y - m_y_speed);
    if(m_y < m_min_y) emit bullet_destroy(this);
}
