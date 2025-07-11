#ifndef ENEMY_H
#define ENEMY_H

#include <QTimer>
#include <QObject>
#include <QDebug>

class QScreen;

class Enemy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int m_index READ get_index WRITE set_index NOTIFY index_changed)

public:
    Enemy(QObject* parent = nullptr);
    int getX() {return m_x;}
    int getY() {return m_y;}
    int get_index() {return m_index;}
    void setX(int x) {m_x = x; emit xChanged();}
    void setY(int y) {m_y = y; emit yChanged();}
    void set_index(int index) {m_index = index;}

signals:
    void xChanged();
    void yChanged();
    void index_changed();

public slots:
    void tick();

private:
    int m_x;
    int m_y;
    int m_min_x;
    int m_max_x;
    int m_min_y;
    int m_max_y;
    int m_index;
    float m_x_speed;
    float m_y_speed;
    QTimer timer_mover;
    QScreen* screen;
};

#endif // ENEMY_H
