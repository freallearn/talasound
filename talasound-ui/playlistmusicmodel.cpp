#include "playlistmusicmodel.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QDebug>
PlaylistMusicModel::PlaylistMusicModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_playlist(0)
{
}


QModelIndex PlaylistMusicModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() && row >= 0 && row < m_playlist->mediaCount() && column >= 0
            && column < ColumnCount ? createIndex(row, column) : QModelIndex();
}

QModelIndex PlaylistMusicModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int PlaylistMusicModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlaylistMusicModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QVariant PlaylistMusicModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if ((index.isValid()) && (role == Qt::DisplayRole)){
        value = m_data[index];
      //  qDebug() << "Value 1 : " << value;
        if(!value.isValid() && index.column() == Title){
            QUrl location = m_playlist->media(index.row()).canonicalUrl();
            value = QFileInfo(location.path()).fileName();
         //   qDebug() << "Value 2 : " << value;
            return value;
        }
        return value;
    }
    value = QVariant();
   // qDebug() << "Value 3 : " << value;
    return value;
}



void PlaylistMusicModel::setPlaylist(QMediaPlaylist *playlist)
{
    if (m_playlist) {
        disconnect(m_playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SLOT(beginInsertItems(int,int)));
        disconnect(m_playlist, SIGNAL(mediaInserted(int,int)), this, SLOT(endInsertItems()));
        disconnect(m_playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SLOT(beginRemoveItems(int,int)));
        disconnect(m_playlist, SIGNAL(mediaRemoved(int,int)), this, SLOT(endRemoveItems()));
        disconnect(m_playlist, SIGNAL(mediaChanged(int,int)), this, SLOT(changeItems(int,int)));
    }

    beginResetModel();
    m_playlist = playlist;

    if (m_playlist) {
        connect(m_playlist, SIGNAL(mediaAboutToBeInserted(int,int)), this, SLOT(beginInsertItems(int,int)));
        connect(m_playlist, SIGNAL(mediaInserted(int,int)), this, SLOT(endInsertItems()));
        connect(m_playlist, SIGNAL(mediaAboutToBeRemoved(int,int)), this, SLOT(beginRemoveItems(int,int)));
        connect(m_playlist, SIGNAL(mediaRemoved(int,int)), this, SLOT(endRemoveItems()));
        connect(m_playlist, SIGNAL(mediaChanged(int,int)), this, SLOT(changeItems(int,int)));
    }

    endResetModel();
}


bool PlaylistMusicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}

void PlaylistMusicModel::beginInsertItems(int start, int end)
{
    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void PlaylistMusicModel::endInsertItems()
{
    endInsertRows();
}

void PlaylistMusicModel::beginRemoveItems(int start, int end)
{
    m_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void PlaylistMusicModel::endRemoveItems()
{
    endInsertRows();
}

void PlaylistMusicModel::changeItems(int start, int end)
{
    m_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}


