#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();
    
    bool startAsServer(int port);
    void connectToServer(const QString& host, int port);
    void sendMessage(const QString& message);
    void closeConnection();

signals:
    void messageReceived(const QString& message);
    void connectionEstablished();
    void connectionLost();

private slots:
    void onNewConnection();
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    QTcpServer* server;
    QTcpSocket* socket;
    bool isServer;
};

#endif // NETWORKMANAGER_H