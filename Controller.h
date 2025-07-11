#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <Bullet.h>
#include <Enemy.h>
#include <QQmlListProperty>
#include <QSoundEffect>

class QScreen;
class QMediaPlayer;
class QAudioOutput;

class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float x READ getX WRITE setX NOTIFY xChanged)
    Q_PROPERTY(float y READ getY WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int score READ get_score WRITE set_score NOTIFY score_changed)
    Q_PROPERTY(QQmlListProperty<Bullet> Q_list_bullets READ Q_list_bullets NOTIFY bulletChanged)
    Q_PROPERTY(QQmlListProperty<Enemy> Q_list_enemies READ Q_list_enemies NOTIFY enemyChanged)

public:
    Controller(QObject* parent = nullptr);

    int getX() {return m_x;}
    int getY() {return m_y;}
    void setX(int x) {m_x = x; emit xChanged();}
    void setY(int y) {m_y = y; emit yChanged();}

    void create_enemy();
    int get_score() { return m_score ;}
    void set_score(int score);

    Q_INVOKABLE void stop_movement();
    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void move_right();
    Q_INVOKABLE void moveUp();
    Q_INVOKABLE void moveDown();
    Q_INVOKABLE void applyThrust();
    Q_INVOKABLE void fireBullet();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE QString show_score();

    // Helps to expose c++ obj or classes to qml
    QQmlListProperty<Bullet> Q_list_bullets(){
        return QQmlListProperty(this, &list_bullets);
    }

    QQmlListProperty<Enemy> Q_list_enemies(){
        return QQmlListProperty(this, &list_enemies);
    }

    static bool is_playing;

public slots:
    void refresh();
    void delete_bullet(Bullet* bullet);
    void delete_enemy(Enemy* enemy);
    void check_collision();
    void update_movement();

signals:
    void xChanged();
    void yChanged();
    void bulletChanged();
    void enemyChanged();
    void score_changed();
    void game_over();
    void game_restart();

private:

    int m_x;
    int m_y;
    int m_min_x;
    int m_max_x;
    int m_min_y;
    int m_max_y;
    int m_max_thrurst;
    int move_direction;
    int lose_height;
    float m_x_speed;
    float m_y_speed;
    float m_max_y_speed = 5;
    float gravity = 0.1;
    QTimer timer;
    QTimer timer_start;
    QTimer timer_movement;
    QList<Bullet*> list_bullets;
    QList<Enemy*> list_enemies;
    QScreen* screen;
    QMediaPlayer* music_player;
    QAudioOutput* audio_output;

    int m_score = 0;

    void game_over(Enemy* enemy);
};

#endif // CONTROLLER_H
