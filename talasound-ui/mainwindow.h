#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QIcon>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "networkpi.h"


#define LIMIT_OF_DURATION 1000

class PlaylistMusicModel;
class QAudioProbe;
class PlayerModel;
class NetworkPI;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    /**
     * @brief addToPlaylist
     * @param urls the urls of the musics
     */
    void addToPlaylist(const QList<QUrl> urls);

    /**
     * @brief isPlayerAvailable
     * @return  true if the player is available otherwise false
     */
    bool isPlayerAvailable() const;

    /**
     * @brief fullScreenChanged
     * @param fullScreen
     */
    void fullScreenChanged(bool fullScreen);


private slots:

    /**
     * @brief on_buttonPlayPause_clicked
     */
    void on_buttonPlayPause_clicked();

    /**
     * @brief on_buttonVolumeONOFF_clicked
     */
    void on_buttonVolumeONOFF_clicked();

    /**
     * @brief on_buttonUpVolume_clicked
     */
    void on_buttonUpVolume_clicked();

    /**
     * @brief on_buttonDownVolume_clicked
     */
    void on_buttonDownVolume_clicked();

    /**
     * @brief open
     */
    void open();

    /**
     * @brief on_actionLire_Musique_triggered
     */
    void on_actionLire_Musique_triggered();



    /**
     * @brief on_listMusicView_activated
     * @param index
     */
    void on_listMusicView_activated(const QModelIndex &index);

    /**
     * @brief playlistPositionChanged
     * @param currentItem
     */
    void playlistPositionChanged(int currentItem);

    /**
     * @brief statusChanged
     * @param status
     */
    void statusChanged(QMediaPlayer::MediaStatus status);

    /**
     * @brief displayErrorMessage
     */
    void displayErrorMessage();

    /**
     * @brief on_playSlider_sliderMoved
     * @param position
     */
    void on_playSlider_sliderMoved(int position);

    /**
     * @brief durationChanged
     * @param duration
     */
    void durationChanged(qint64 duration);

    /**
     * @brief positionChanged
     * @param progress
     */
    void positionChanged(qint64 progress);

    /**
     * @brief setState
     * @param state
     */
    void setState(QMediaPlayer::State state);

private:
    /**
     * @brief configureConnections
     * create connection with signals and slots
     */
    void configureConnections(void);
    void clearHistogram();
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);
    void jump(const QModelIndex &index);

    void seek(int seconds);
    Ui::MainWindow *ui;
    /**
     * @brief player
     * Joue la musique
     */
    QMediaPlayer *player;

    /**
     * @brief playlist
     * Playlist de musique
     */
    QMediaPlaylist *playlist;
    PlaylistMusicModel *playlistModel;
    PlayerModel *playerModel;
    QString trackInfo;
    QString statusInfo;

    QAudioProbe *audioProbe;
    /**
     * @brief networkPI
     * Connexion avec la pi
     */
    NetworkPI *networkPI;

};

#endif // MAINWINDOW_H
