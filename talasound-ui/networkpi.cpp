#include "networkpi.h"

#include <QtWidgets>
#include <QtNetwork>
#include <QTextBrowser>
#include <QString>
#include <QDebug>
#include <QTcpServer>

NetworkPI::NetworkPI(QTextBrowser *logText):tcpServer(Q_NULLPTR),networkSession(0)
{

    QNetworkConfigurationManager manager;

    setLog(logText);
    if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }
        networkSession = new QNetworkSession(config,this);
        connect(networkSession, &QNetworkSession::opened, this, &NetworkPI::sessionOpened);
        connect(tcpServer, &QTcpServer::newConnection, this, &NetworkPI::hadConnection);
        networkSession->open();

    }
    else{
        sessionOpened();
    }
}


void NetworkPI::sendStart(QString nameOfMusique)
{
    QString chaine("start:"+nameOfMusique);
    sendInstructionToPi(chaine);
}

void NetworkPI::sendStop()
{
    sendInstructionToPi("stop");
}

void NetworkPI::sendPause()
{
    sendInstructionToPi("pause");
}

void NetworkPI::setLog(QTextBrowser *logText)
{
    this->logText=logText;
}

void NetworkPI::sessionOpened()
{
    // Save the used configuration
    qDebug() << "Session Opened";
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice){
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        }
        else{
            id = config.identifier();
        }

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress("192.168.1.18"),8888)) {

        logText->append(QString("Impossible de démarrer le serveur: %1\n").arg(tcpServer->errorString()));
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            qDebug() << "IP 1 : " << ipAddress;
            break;
        }
    }

    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty()){
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        qDebug() << "IP 2 : " << ipAddress;

        qDebug() << "Lance";
    }
    logText->append(QString("Le serveur tourne sur l'adresse \n\nIP: %1\nport: %2\n\n Attente de connexion de la PI\n")
                    .arg(ipAddress).arg(tcpServer->serverPort()));

}

void NetworkPI::hadConnection()
{
   logText->append(QString("PI Connectée"));
}

void NetworkPI::sendInstructionToPi(QString instruction)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out << instruction;
    qDebug() << "Send";
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    qDebug() << "Envoi";
    // connect(clientConnection, &QAbstractSocket::disconnected,
    //       clientConnection, &QObject::deleteLater);
    if(clientConnection){
        clientConnection->write(block);
        clientConnection->disconnectFromHost();
        logText->append(instruction);
    }
}
