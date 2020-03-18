#ifndef NETWORKPI_H
#define NETWORKPI_H


#include <QString>
#include <QTextBrowser>

class QTextBrowser;
class QTcpServer;
class QNetworkSession;

/**
 * @brief The NetworkPI class
 */
class NetworkPI : public QObject{

public:
    /**
     * @brief NetworkPI
     * @param logText the log text
     */
    explicit NetworkPI(QTextBrowser *logText);

    /**
     * @brief sendStart
     * @param nameOfMusique the name of musique to start
     */
    void sendStart(QString nameOfMusique);

    /**
     * @brief sendStop
     */
    void sendStop();

    /**
     * @brief sendPause
     */
    void sendPause();

    /**
     * @brief setLog
     * @param logText the log text
     */
    void setLog(QTextBrowser *logText);

private slots:

    /**
     * @brief sessionOpened
     */
    void sessionOpened();
private:

    /**
     * @brief tcpServer
     */
    QTcpServer *tcpServer;

    /**
     * @brief networkSession
     */
    QNetworkSession *networkSession;

    /**
     * @brief sendInstructionToPi
     * @param instruction the instruction to send
     */
    void sendInstructionToPi(QString instruction);

    /**
     * @brief logText
     */
    QTextBrowser *logText;
};

#endif // NETWORKPI_H
