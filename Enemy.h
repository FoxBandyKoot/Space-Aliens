#ifndef ENEMY_H
#define ENEMY_H

#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QSoundEffect>

class QScreen;

class Enemy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int m_index READ get_index WRITE set_index NOTIFY index_changed)
    Q_PROPERTY(bool m_is_destroyed READ is_destroyed WRITE set_destroyed NOTIFY destroyed_changed)

public:
    Enemy(QObject* parent = nullptr);

    int getX() {return m_x;}
    int getY() {return m_y;}
    int get_index() {return m_index;}
    const QVector<QUrl>& get_audio_files_destruction() {return audio_files_destruction;}

    bool is_destroyed() const { return m_is_destroyed; }
    void set_destroyed(bool value);
    void setX(int x) {m_x = x; emit xChanged();}
    void setY(int y) {m_y = y; emit yChanged();}
    void set_index(int index) {m_index = index;}


signals:
    void destroyed_changed();
    void xChanged();
    void yChanged();
    void index_changed();

public slots:
    void tick();

private:
    bool m_is_destroyed = false;
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
    QVector<QSoundEffect*> sfx_destructions;
    QVector<QUrl> audio_files_destruction;
};

#endif // ENEMY_H
