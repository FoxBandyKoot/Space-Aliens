#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <QUrl>
#include <functional>


class QMediaPlayer;
class QAudioOutput;

// Singleton to handle audio
class SoundManager : public QObject
{
    Q_OBJECT
public:
    static SoundManager* get_singleton();

    void play_then(const QUrl& source, std::function<void()> onFinished = nullptr);
    void set_volume(float volume); // 0.0 to 1.0
    void play_theme();
    float get_volume() const;


private:
    float global_volume = 0.2f;
    QMediaPlayer* music_player;
    QAudioOutput* audio_output;
};

#endif // SOUNDMANAGER_H
