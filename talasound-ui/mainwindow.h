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

/**
 * @brief The MainWindow class
 * The mainwindow the application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow
     * Constructor
     * @param parent the parent the main window by deflaut is none
     */
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
    void on_actionLire_Musique_triggered(void);



    /**
     * @brief on_listMusicView_activated
     * @param index the index of the model
     */
    void on_listMusicView_activated(const QModelIndex &index);

    /**
     * @brief playlistPositionChanged
     * @param currentItem the item selected to playe
     */
    void playlistPositionChanged(int currentItem);

    /**
     * @brief statusChanged
     * @param status the status of the media
     */
    void statusChanged(QMediaPlayer::MediaStatus status);

    /**
     * @brief displayErrorMessage
     */
    void displayErrorMessage(void);

    /**
     * @brief on_playSlider_sliderMoved
     * @param position the position
     */
    void on_playSlider_sliderMoved(int position);

    /**
     * @brief durationChanged
     * @param duration the duration of the new music
     */
    void durationChanged(qint64 duration);

    /**
     * @brief positionChanged
     * @param progress the progress of duration of the music
     */
    void positionChanged(qint64 progress);

    /**
     * @brief setState
     * @param state the state of media player
     */
    void setState(QMediaPlayer::State state);

private:
    /**
     * @brief configureConnections
     * create connection with signals and slots
     */
    void configureConnections(void);
    void clearHistogram(void);
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

    /**
     * @brief playlistModel
     * Model of the playlsite
     */
    PlaylistMusicModel *playlistModel;
    /**
     * @brief playerModel
     * the model of the player
     */
    PlayerModel *playerModel;
    /**
     * @brief trackInfo
     * info of the track
     */
    QString trackInfo;

    /**
     * @brief statusInfo
     */
    QString statusInfo;

    QAudioProbe *audioProbe;
    /**
     * @brief networkPI
     * Connexion avec la pi
     */
    NetworkPI *networkPI;

};

#endif // MAINWINDOW_H
