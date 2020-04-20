#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistmusicmodel.h"
#include "playermodel.h"
#include "networkpi.h"

#include <QStyle>
#include <QFileDialog>
#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QDebug>
#include <QProcess>
#include <string>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    networkPI = new NetworkPI(ui->logTextBrowser);
    playerModel = new PlayerModel();
    player = new QMediaPlayer(this);
    // owned by PlaylistModel
    playlist = new QMediaPlaylist();

    player->setPlaylist(playlist);


    playlistModel = new PlaylistMusicModel(this);
    playlistModel->setPlaylist(playlist);
    ui->listMusicView->setModel(playlistModel);

    ui->listMusicView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));

    ui->buttonPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->buttonVolumeONOFF->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->playSlider->setRange(0, player->duration() / LIMIT_OF_DURATION);
    audioProbe = new QAudioProbe(this);
    audioProbe->setSource(player);
    launchSignal = new QProcess(this);
    launchSignal->setReadChannel(QProcess::StandardOutput);
    configureConnections();
    //launchSignal->start("/usr/bin/python3 musicProcessing.py");
}



MainWindow::~MainWindow()
{
    delete ui;
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void MainWindow::addToPlaylist(const QList<QUrl> urls)
{
    foreach (const QUrl &url, urls) {
        if (isPlaylist(url)){
            playlist->load(url);
        }
        else{
            qDebug() << "Url : " << url.toLocalFile();
            launchComputing(url.toLocalFile());
            playlist->addMedia(url);
        }
    }
}

bool MainWindow::isPlayerAvailable() const
{
    return player->isAvailable();
}

void MainWindow::readSortie()
{
    std::string sorite = launchSignal->readAllStandardOutput().toStdString();

    QString sortieS = QString::fromStdString(sorite);
    qDebug() << "Log : " << sortieS;
    ui->logTextBrowser->append(sortieS);

}

void MainWindow::giveStateString(QProcess::ProcessState state)
{
    qDebug() << "State  ?  : " << state;
    if(state==QProcess::NotRunning){
       // networkPI->sendFileCommande("./orders.csv");
    }
}

void MainWindow::sendFinished()
{
   ui->logTextBrowser->append(QString("fini"));
}

void MainWindow::on_buttonPlayPause_clicked()
{
    qDebug() << "Begin";
    qDebug() << "Change State :  " << playerModel->getPlayerState();
    if(playerModel->isPlaying()) {
        qDebug() << "Play";

        player->pause();
        networkPI->sendPause();
    }
    else {
        qDebug() << "Pause";
        //networkPI->sendFileCommande("");
        player->play();
        networkPI->sendStart(playlist->currentMedia().canonicalUrl().fileName());
    }
}

void MainWindow::on_buttonVolumeONOFF_clicked()
{
    playerModel->changeMuted();
    player->setMuted(playerModel->getMutedPlayer());
    ui->buttonVolumeONOFF->setIcon((playerModel->getMutedPlayer()) ? style()->standardIcon(QStyle::SP_MediaVolumeMuted) : style()->standardIcon(QStyle::SP_MediaVolume));
}

void MainWindow::on_buttonUpVolume_clicked()
{
    playerModel->upVolume();
}

void MainWindow::on_buttonDownVolume_clicked()
{
    playerModel->downVolume();
    // ui->buttonVolumeONOFF->setIcon(isMuted ? style()->standardIcon(QStyle::SP_MediaVolumeMuted) : style()->standardIcon(QStyle::SP_MediaVolume));
}


void MainWindow::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        networkPI->sendStart(playlist->currentMedia().canonicalUrl().fileName());
        player->play();
    }
}

void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * LIMIT_OF_DURATION);
}

void MainWindow::open(){
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted){
        addToPlaylist(fileDialog.selectedUrls());
    }
}

void MainWindow::on_actionLire_Musique_triggered()
{
    open();
}


void MainWindow::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);
    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}


void MainWindow::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

void MainWindow::setStatusInfo(const QString &info)
{
    playerModel->setStatusInfo(info);
    setWindowTitle(playerModel->getTitleInfo());
}

void MainWindow::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
            status == QMediaPlayer::BufferingMedia ||
            status == QMediaPlayer::StalledMedia){
        setCursor(QCursor(Qt::BusyCursor));
    }
    else{
        unsetCursor();
    }
#endif
}


void MainWindow::on_listMusicView_activated(const QModelIndex &index)
{
    jump(index);
}

void MainWindow::playlistPositionChanged(int currentItem)
{
    ui->listMusicView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

void MainWindow::on_playSlider_sliderMoved(int position)
{
    seek(position);
}

void MainWindow::durationChanged(qint64 duration)
{
    playerModel->setDuration(duration);
    ui->playSlider->setMaximum(playerModel->getDuration());
}

void MainWindow::positionChanged(qint64 progress)
{
    qint64 normProgress = progress / LIMIT_OF_DURATION;
    if (!ui->playSlider->isSliderDown()) {
        ui->playSlider->setValue(normProgress);
    }
    updateDurationInfo(normProgress);
}

void MainWindow::setState(QMediaPlayer::State state)
{
    if(state!=playerModel->getPlayerState()){
        playerModel->setState(state);
        ui->buttonPlayPause->setIcon(style()->standardIcon(playerModel->isPlaying() ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay));
    }
}

void MainWindow::launchComputing(QString nameOfMusique)
{
    QString program = "musicProcessing.py";
    QStringList argument;
    argument << program << nameOfMusique;
    //launchSignal->start("/usr/bin/python3 musicProcessing.py");

    launchSignal->start("/usr/bin/python3",argument);
}

void MainWindow::configureConnections()
{
    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(setState(QMediaPlayer::State)));
    connect(launchSignal,SIGNAL(readyReadStandardOutput()),SLOT(readSortie()));
    connect(launchSignal,SIGNAL(stateChanged(QProcess::ProcessState)),SLOT(giveStateString(QProcess::ProcessState)));
    //connect(launchSignal,SIGNAL(),SLOT(giveStateString(QProcess::ProcessState)));
}


void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    ui->labelDuration->setText(playerModel->giveDuration(currentInfo));
}
