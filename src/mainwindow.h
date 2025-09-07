#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <memory>
#include "networkmanager.h"
#include "protocolhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartAsAlice();
    void onStartAsBob();
    void onStopProtocol();
    void onMessageReceived(const QString& message);
    void onConnectionEstablished();
    void onConnectionLost();
    void appendLog(const QString& message);
    void onProtocolStep();
    void onClearLog();

private:
    void initializeProtocol();
    void loadConfig();
    void setupPremiumStyle();
    
    Ui::MainWindow *ui;
    std::unique_ptr<NetworkManager> networkManager;
    std::unique_ptr<ProtocolHandler> protocolHandler;
    QTimer* protocolTimer;
    
    bool isAlice;
    bool protocolRunning;
    int currentRound;
    int maxRounds;
    int sleepDuration;
    
    QString aliceIP;
    int alicePort;
    QString bobIP;
    int bobPort;
};

#endif // MAINWINDOW_H
