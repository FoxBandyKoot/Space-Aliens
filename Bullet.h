#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QTimer>
#include <QDebug>

class Controller;

class Bullet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY yChanged)

public:
    Bullet(Controller* controller, QObject* parent = nullptr);
    int getX() {return m_x;}
    int getY() {return m_y;}
    void setX(int x) {m_x = x; emit xChanged();}
    void setY(int y) {m_y = y; emit yChanged();}

signals:
    void xChanged();
    void yChanged();
    void bullet_destroy(Bullet* bullet);

public slots:
    void updateBullet();

private:
    int m_x;
    int m_y;
    int m_min_x;
    int m_min_y;
    float m_x_speed;
    float m_y_speed;
    QTimer timer;
};

#endif // BULLET_H
