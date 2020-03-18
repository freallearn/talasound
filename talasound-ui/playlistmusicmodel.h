#ifndef PLAYLISTMUSICMODEL_H
#define PLAYLISTMUSICMODEL_H

#include <QAbstractItemModel>


class QMediaPlaylist;

/**
 * @brief The PlaylistMusicModel class
 */
class PlaylistMusicModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    /**
     * @brief The Column enum
     */
    enum Column{
        Title =0,
        ColumnCount
    };
    /**
     * @brief PlaylistMusicModel
     * @param parent
     */
    explicit PlaylistMusicModel(QObject *parent = nullptr);


    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    /**
     * @brief setPlaylist
     * @param playlist
     */
    void setPlaylist(QMediaPlaylist *playlist);
private slots:

    /**
     * @brief beginInsertItems
     * @param start
     * @param end
     */
    void beginInsertItems(int start, int end);

    /**
     * @brief endInsertItems
     */
    void endInsertItems();

    /**
     * @brief beginRemoveItems
     * @param start
     * @param end
     */
    void beginRemoveItems(int start, int end);

    /**
     * @brief endRemoveItems
     */
    void endRemoveItems();

    /**
     * @brief changeItems
     * @param start
     * @param end
     */
    void changeItems(int start, int end);
private:

    /**
     * @brief m_playlist
     */
    QMediaPlaylist *m_playlist;

    /**
     * @brief m_data
     */
    QMap<QModelIndex, QVariant> m_data;
};

#endif // PLAYLISTMUSICMODEL_H
