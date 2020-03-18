#include "playermodel.h"

#include <QDebug>
PlayerModel::PlayerModel():
    playerState(QMediaPlayer::StoppedState),
    playerMuted(false),
    volume(DEFAULT_VOLUME)

{
    qDebug() << "State Player" << playerState;

}

QMediaPlayer::State PlayerModel::getPlayerState() const
{
    return playerState;
}

int PlayerModel::getVolume() const
{
    return volume;
}

bool PlayerModel::getMutedPlayer() const
{
    return playerMuted;
}

void PlayerModel::setState(QMediaPlayer::State state)
{
    playerState = (state != playerState) ? state : playerState;
}

void PlayerModel::setTrackInfo(const QString &trackInfo)
{
    this->trackInfo = trackInfo;
}

void PlayerModel::setStatusInfo(const QString &info)
{
    trackInfo = info;
}

QString PlayerModel::getTitleInfo()
{
    return (statusInfo.isEmpty()) ? trackInfo : QString("%1 | %2").arg(trackInfo).arg(statusInfo);
}

qint64 PlayerModel::getDuration() const
{
    return duration;
}

QString PlayerModel::giveDuration(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime = computeTime(currentInfo);
        QTime totalTime = computeTime(duration);
        QString format = duration > 3600 ? "hh:mm:ss" : "mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    return tStr;
}

void PlayerModel::correctVolume()
{
    volume = (volume>=MAX_VOLUME) ? MAX_VOLUME : volume;
    volume = (volume<=MIN_VOLUME) ? MIN_VOLUME : volume;
}

QTime PlayerModel::computeTime(qint64 time)
{
    return QTime((time/3600)%60, (time/60)%60, time%60, (time*1000)%1000);
}

bool PlayerModel::isPlaying()
{
    qDebug() << "ISPlaying" << playerState;
    return (playerState==QMediaPlayer::PlayingState);
}

void PlayerModel::changePlayState()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        playerState = QMediaPlayer::PlayingState;
        break;
    case QMediaPlayer::PlayingState:
        playerState = QMediaPlayer::PausedState;
        break;
    }
}

void PlayerModel::setVolume(int volume)
{
    logarithmicVolume = QAudio::convertVolume(volume / qreal(MAX_VOLUME),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);
    this->volume = volume;
}

void PlayerModel::upVolume(int volume)
{
    this->volume+=volume;
}

void PlayerModel::downVolume(int volume)
{
    this->volume-=volume;
}

void PlayerModel::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;
    }
}

void PlayerModel::changeMuted()
{
    playerMuted=!playerMuted;
}

void PlayerModel::setDuration(qint64 duration)
{
    this->duration=duration/LIMIT_OF_DURATION;
}
