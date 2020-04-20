#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QMediaPlayer>
#include <QString>
#include <QTime>


#define MAX_VOLUME 100
#define MIN_VOLUME 0
#define DEFAULT_VOLUME 50
#define LIMIT_OF_DURATION 1000

/**
 * @brief The PlayerModel class
 * @author Fréalle Arnaud
 * The model of the player contains data of the player
 */
class PlayerModel
{
public:
    /**
     * @brief PlayerModel
     * Constructor of the player Model
     */
    PlayerModel();

    /**
     * @brief getPlayerState
     * @return the current player state
     */
    QMediaPlayer::State getPlayerState() const;

    /**
     * @brief getVolume
     * @return the current volume
     */
    int getVolume() const;
    /**
     * @brief getMutedPlayer
     * @return the muted
     */
    bool getMutedPlayer() const;

    /**
     * @brief isPlaying
     * @return true if the player is playing false otherwise
     */
    bool isPlaying();

    /**
     * @brief changePlayState
     */
    void changePlayState();
    /**
     * @brief setVolume
     * @param volume the volume to up
     */
    void setVolume(int volume);

    /**
     * @brief upVolume
     * @param volume the volume to down
     */
    void upVolume(int volume=1);

    /**
     * @brief downVolume
     * @param volume the volume to down
     */
    void downVolume(int volume=1);
    /**
     * @brief setMuted
     * @param muted the new muted
     */
    void setMuted(bool muted);

    /**
     * @brief changeMuted
     * Switch the muted state
     */
    void changeMuted();

    /**
     * @brief setDuration
     * Set the duration
     * @param duration the new duration
     */
    void setDuration(qint64 duration);
    /**
     * @brief setState
     * @param state the new state
     */
    void setState(QMediaPlayer::State state);


    /**
     * @brief setTrackInfo
     * @param trackInfo the new track info
     */
    void setTrackInfo(const QString &trackInfo);

    /**
     * @brief setStatusInfo
     * @param info the new status info
     */
    void setStatusInfo(const QString &info);

    /**
     * @brief getTitleInfo
     * @return  the title info
     */
    QString getTitleInfo();

    /**
     * @brief getDuration
     * @return the current duration
     */
    qint64 getDuration() const;

    /**
     * @brief giveDuration
     * @param currentInfo the currentinfo of the time
     * @return a string of duration
     */
    QString giveDuration(qint64 currentInfo);
private:

    /**
     * @brief playerState
     * the current player state
     */
    QMediaPlayer::State playerState;
    /**
     * @brief playerMuted
     */
    bool playerMuted;
    /**
     * @brief volume
     * the current volume
     */
    int volume;

    /**
     * @brief duration
     * Durée de la musique
     */
    qint64 duration;

    /**
     * @brief statusInfo
     */
    QString statusInfo;

    /**
     * @brief trackInfo
     */
    QString trackInfo;

    /**
     * @brief logarithmicVolume
     */
    qreal logarithmicVolume;
    /**
     * @brief correctVolume
     * correct the volume if its out of the bound
     */
    void correctVolume();

    /**
     * @brief computeTime
     * @param time the time to convert in QTime
     * @return the time compute
     */
    QTime computeTime(qint64 time);
};

#endif // PLAYERMODEL_H
