#include "Controller.h"
#include <QGuiApplication>
#include <QSCreen>
#include <QDebug>
#include <QRandomGenerator>
#include <SoundManager.h>

bool Controller::is_playing = true;

Controller::Controller(QObject* parent): m_x_speed(8), m_max_thrurst(-3), m_y_speed(1) {

    SoundManager::get_singleton()->play_theme();

    screen = QGuiApplication::primaryScreen();
    setX(screen->geometry().width() / 2);
    setY(screen->geometry().height() / 2);

    m_min_x = (screen->geometry().width() / 100) * 10;
    m_max_x = (screen->geometry().width() / 100) * 90;

    m_min_y = (screen->geometry().height() / 100) * 10;
    m_max_y = (screen->geometry().height()/ 100) * 80;

    lose_height = screen->geometry().height();

    connect(&timer, &QTimer::timeout, this, &Controller::refresh); // Gravity and bullets collisions
    timer.start(16); // 16 milliseconds, apply 60 times per seconds

    connect(&timer_start, &QTimer::timeout, this, &Controller::create_enemy); // Spawn enemy
    create_enemy();

    connect(&timer_movement, &QTimer::timeout, this, &Controller::update_movement); // Continuous movement
    timer_movement.start(16); // 16 milliseconds, apply 60 times per seconds
}

void Controller::refresh(){
    if(is_playing == false) return;

    // Handle gravity
    m_y += m_y_speed;
    m_y_speed += gravity;
    if(m_y_speed > m_max_y_speed) m_y_speed = m_max_y_speed;
    if(m_y > m_max_y) m_y = m_max_y;
    emit yChanged();

    // Handle game over
    for(Enemy* enemy: list_enemies)
    {
        if(enemy->is_destroyed() == true) continue;
        if(enemy->getY() > lose_height){ // Ennemy has crossed the field
            game_over(enemy);
            return;
        }

        // Handdle bullets collisions with enemies
        check_collision();

        int player_left_side = getX();
        int player_right_side = getX() + 85; // 85 is the player width
        int player_top_side = getY();
        int player_bottom_side = getY() + 85; // 85 is the player height

        int enemy_left_side = enemy->getX();
        int enemy_right_side = enemy->getX() + 45; // 45 is the enemy width
        int enemy_top_side = enemy->getY();
        int enemy_bottom_side = enemy->getY() + 45; // 45 is the enemy height

        // Player hit by ennemy
        if(enemy_right_side > player_left_side &&
            enemy_left_side < player_right_side &&
            enemy_top_side < player_bottom_side &&
            enemy_bottom_side > player_top_side)
        {
            SoundManager::get_singleton()->play_then(QUrl::fromLocalFile(":/sounds/audio/explosion_player.wav"));
            game_over(enemy);
            return;
        }
    }
}

// Continuous movement, called every 16 milliseconds
void Controller::update_movement(){
    if(is_playing == false) return;

    if(move_direction == -1 && m_x > m_min_x) setX(m_x - m_x_speed);  // Go left while we are pushing left
    if(move_direction == 1 && m_x < m_max_x) setX(m_x + m_x_speed); // Go right while we are pushing right
}

void Controller::stop_movement(){
    if(is_playing == false) return;
    move_direction = 0;
    timer_movement.stop();
}

void Controller::moveLeft() {
    if(is_playing == false) return;

    move_direction = -1;
    if(!timer_movement.isActive()){
        timer_movement.start();
    }
}

void Controller::move_right() {
    if(is_playing == false) return;

    move_direction = 1;
    if(!timer_movement.isActive()){
        timer_movement.start();
    }
}

void Controller::moveUp() {
    if(is_playing == false) return;

    setY(m_y - m_y_speed);
    if(m_y < m_min_y) setY(m_min_y);
}

void Controller::moveDown() {
    if(is_playing == false) return;

    setY(m_y + m_y_speed);
    if(m_y > m_max_y) setY(m_max_y);
}

void Controller::applyThrust(){
    if(is_playing == false) return;

    m_y_speed = m_max_thrurst;
    if(m_y < m_max_y/4) m_y_speed = m_max_y_speed;
}

void Controller::fireBullet(){
    if(is_playing == false) return;

    Bullet* bullet = new Bullet(this, this);
    bullet->setX(m_x + 40);
    bullet->setY(m_y - 17);
    list_bullets.append(bullet);

    SoundManager::get_singleton()->play_then(QUrl::fromLocalFile(":/sounds/audio/mixkit-short-laser-gun-shot-1670.wav"));
    emit bulletChanged();
}

void Controller::game_over(Enemy* enemy){
    int idx = list_enemies.indexOf(enemy);
    list_enemies.removeAt(idx);
    enemy->deleteLater();

    is_playing = false;
    emit game_over();
}

void Controller::restartGame()
{
    for(Enemy* enemy: list_enemies) delete enemy;
    list_enemies.clear();

    for(Bullet* bullet: list_bullets) delete bullet;
    list_bullets.clear();

    emit enemy_changed();
    emit bulletChanged();

    setX(screen->geometry().width() / 2);
    setY(screen->geometry().height() / 2);

    set_score(0);
    move_direction = 0;
    is_playing = true;
    create_enemy();
    emit xChanged();
    emit yChanged();
    emit game_restart();
}

QString Controller::show_score(){ return QString::number(get_score()); }

void Controller::create_enemy(){
    if(is_playing == false) return;

    Enemy* enemy = new Enemy(this);
    list_enemies.append(enemy);
    emit enemy_changed();

    // increase spawn speed
    int spawn_time = 3000 - m_score * 100;
    if(spawn_time < 300) spawn_time = 300;
    timer_start.start(spawn_time);
}

void Controller::set_score(int score){
    m_score = score;
    emit score_changed();
}

void Controller::delete_bullet(Bullet* bullet)
{
    int idx = list_bullets.indexOf(bullet);
    if(idx != -1)
    {
        list_bullets.removeAt(idx);
        bullet->deleteLater();
        emit bulletChanged();
    }
}

void Controller::delete_enemy(Enemy* enemy)
{
    int idx = list_enemies.indexOf(enemy);
    if(idx != -1)
    {
        list_enemies.removeAt(idx);
        enemy->deleteLater();
    }
}

void Controller::check_collision(){
    for(int i = list_bullets.size() - 1; i >= 0; i--)
    {
        Bullet* bullet = list_bullets[i];
        for(int j = list_enemies.size() - 1; j >= 0; j --)
        {
            Enemy* enemy = list_enemies[j];
            if(enemy->is_destroyed() == true) continue;
            int bullet_left_side = bullet->getX();
            int bullet_right_side = bullet->getX() + 4; // 4 is the bullet width
            int bullet_top_side = bullet->getY();
            int bullet_bottom_side = bullet->getY() + 12; // 12 is the bullet height

            int enemy_left_side = enemy->getX();
            int enemy_right_side = enemy->getX() + 45; // 4 is the enemy width
            int enemy_top_side = enemy->getY();
            int enemy_bottom_side = enemy->getY() + 45; // 45 is the enemy height

            if(bullet_right_side > enemy_left_side &&
                bullet_left_side < enemy_right_side &&
                bullet_top_side < enemy_bottom_side &&
                bullet_bottom_side > enemy_top_side)
            {
                // ENEMY HITTED
                enemy->set_destroyed(true);  // Hide ennemy and stop movement
                emit enemy_changed();

                QVector<QUrl> sfx_destructions = enemy->get_audio_files_destruction();
                if (!sfx_destructions.isEmpty()) {
                    int idx = QRandomGenerator::global()->bounded(sfx_destructions.size());
                    SoundManager::get_singleton()->play_then(
                        sfx_destructions[idx],
                        [this, enemy]() { delete_enemy(enemy); }
                        );
                }

                delete_bullet(bullet);
                set_score(get_score() + 1);
                break;
            }
        }
    }
}

