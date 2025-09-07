#include "networkmanager.h"
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent)
    , server(nullptr)
    , socket(nullptr)
    , isServer(false)
{
}

NetworkManager::~NetworkManager()
{
    closeConnection();
}

bool NetworkManager::startAsServer(int port)
{
    isServer = true;
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &NetworkManager::onNewConnection);
    
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start:" << server->errorString();
        return false;
    }
    
    qDebug() << "Server listening on port" << port;
    return true;
}

void NetworkManager::connectToServer(const QString& host, int port)
{
    isServer = false;
    socket = new QTcpSocket(this);
    
    connect(socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
    
    socket->connectToHost(host, port);
}

void NetworkManager::onNewConnection()
{
    socket = server->nextPendingConnection();
    
    connect(socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, &NetworkManager::onError);
    
    emit connectionEstablished();
}

void NetworkManager::onConnected()
{
    qDebug() << "Connected to server";
    emit connectionEstablished();
}

void NetworkManager::onDisconnected()
{
    qDebug() << "Disconnected";
    emit connectionLost();
}

void NetworkManager::onReadyRead()
{
    if (!socket) return;
    
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    emit messageReceived(message);
}

void NetworkManager::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error:" << error;
    if (socket) {
        qDebug() << socket->errorString();
    }
}

void NetworkManager::sendMessage(const QString& message)
{
    if (!socket || socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "Cannot send message: socket not connected";
        return;
    }
    
    socket->write(message.toUtf8());
    socket->flush();
}

void NetworkManager::closeConnection()
{
    if (socket) {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
    
    if (server) {
        server->close();
        server->deleteLater();
        server = nullptr;
    }
}