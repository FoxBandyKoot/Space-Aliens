#include "SoundManager.h"
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>

SoundManager* SoundManager::get_singleton()
{
    static SoundManager instance_singleton;
    return &instance_singleton;
}
void SoundManager::set_volume(float volume)
{
    global_volume = qBound(0.0f, volume, 1.0f);
}

float SoundManager::get_volume() const
{
    return global_volume;
}

void SoundManager::play_then(const QUrl& source, std::function<void()> onFinished)
{
    QSoundEffect* sfx = new QSoundEffect(this);
    sfx->setSource(source);
    sfx->setVolume(global_volume);
    sfx->play();

    // Clean up & callback when sound finishes
    connect(sfx, &QSoundEffect::playingChanged, this, [sfx, onFinished]() {
        if (!sfx->isPlaying()) {
            sfx->deleteLater();
            if (onFinished){ onFinished(); }
        }
    });
}

void SoundManager::play_theme(){
    music_player = new QMediaPlayer(this);
    audio_output = new QAudioOutput(this);
    music_player->setAudioOutput(audio_output);

    music_player->setSource(QUrl("qrc:/sounds/audio/L'Horizon d'Ambre.wav"));
    music_player->setLoops(QMediaPlayer::Infinite);
    audio_output->setVolume(0.25);

    music_player->play();
}
