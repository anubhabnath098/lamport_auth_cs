#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , protocolRunning(false)
    , currentRound(0)
    , maxRounds(99)
    , sleepDuration(1000)
{
    ui->setupUi(this);

    // Load configuration
    loadConfig();

    // Setup protocol timer
    protocolTimer = new QTimer(this);
    connect(protocolTimer, &QTimer::timeout, this, &MainWindow::onProtocolStep);

    // Connect buttons
    connect(ui->btnStartAlice, &QPushButton::clicked, this, &MainWindow::onStartAsAlice);
    connect(ui->btnStartBob, &QPushButton::clicked, this, &MainWindow::onStartAsBob);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::onStopProtocol);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearLog);

    // Initial UI state
    ui->btnStop->setEnabled(false);
    ui->progressBar->setMaximum(maxRounds);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadConfig()
{
    QSettings settings("config.ini", QSettings::IniFormat);

    // Network config
    settings.beginGroup("Network");
    aliceIP = settings.value("AliceIP", "127.0.0.1").toString();
    alicePort = settings.value("AlicePort", 8080).toInt();
    bobIP   = settings.value("BobIP", "127.0.0.1").toString();
    bobPort = settings.value("BobPort", 8081).toInt();
    settings.endGroup();

    // Protocol config
    settings.beginGroup("Protocol");
    maxRounds     = settings.value("ChainLength", 100).toInt() - 1;
    sleepDuration = settings.value("SleepDuration", 1000).toInt();
    settings.endGroup();

    // Update UI
    ui->lblConfig->setText(
        QString("Alice: %1:%2 | Bob: %3:%4")
            .arg(aliceIP).arg(alicePort)
            .arg(bobIP).arg(bobPort)
    );
}

void MainWindow::onStartAsAlice()
{
    isAlice = true;
    initializeProtocol();

    networkManager = std::make_unique<NetworkManager>(this);
    connect(networkManager.get(), &NetworkManager::messageReceived,
            this, &MainWindow::onMessageReceived);
    connect(networkManager.get(), &NetworkManager::connectionEstablished,
            this, &MainWindow::onConnectionEstablished);

    if (networkManager->startAsServer(alicePort)) {
        appendLog("Started as Alice on port " + QString::number(alicePort));
        ui->btnStartAlice->setEnabled(false);
        ui->btnStartBob->setEnabled(false);
        ui->btnStop->setEnabled(true);
        ui->lblRole->setText("Role: ALICE (Verifier)");
        protocolRunning = true;
    } else {
        appendLog("Failed to start as Alice");
        QMessageBox::critical(this, "Error",
            "Failed to start server on port " + QString::number(alicePort));
    }
}

void MainWindow::onStartAsBob()
{
    isAlice = false;
    initializeProtocol();

    networkManager = std::make_unique<NetworkManager>(this);
    connect(networkManager.get(), &NetworkManager::messageReceived,
            this, &MainWindow::onMessageReceived);
    connect(networkManager.get(), &NetworkManager::connectionEstablished,
            this, &MainWindow::onConnectionEstablished);

    networkManager->connectToServer(aliceIP, alicePort);
    appendLog("Started as Bob, connecting to Alice at " + aliceIP + ":" +
              QString::number(alicePort));

    ui->btnStartAlice->setEnabled(false);
    ui->btnStartBob->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->lblRole->setText("Role: BOB (Prover)");
    protocolRunning = true;
}

void MainWindow::initializeProtocol()
{
    protocolHandler = std::make_unique<ProtocolHandler>(isAlice, maxRounds + 1);
    currentRound = 0;
    ui->progressBar->setValue(0);
}

void MainWindow::onConnectionEstablished()
{
    appendLog("Connection established!");
    if (isAlice) {
        protocolTimer->start(sleepDuration);
    }
}

void MainWindow::onProtocolStep()
{
    if (!protocolRunning || currentRound >= maxRounds) {
        protocolTimer->stop();
        if (currentRound >= maxRounds) {
            appendLog("Protocol completed successfully! All " +
                      QString::number(maxRounds) + " rounds finished.");
            onStopProtocol();
        }
        return;
    }

    if (isAlice) {
        currentRound++;
        QString challenge = protocolHandler->generateChallenge(currentRound);
        networkManager->sendMessage("CHALLENGE:" + challenge);
        appendLog(QString("[Round %1] Sent challenge: %2")
                      .arg(currentRound)
                      .arg(challenge));
        ui->progressBar->setValue(currentRound);
    }
}

void MainWindow::onMessageReceived(const QString &message)
{
    if (message.startsWith("CHALLENGE:") && !isAlice) {
        QString challenge = message.mid(10);
        QString response = protocolHandler->generateResponse(challenge);
        networkManager->sendMessage("RESPONSE:" + response);
        appendLog("Received challenge: " + challenge);
        appendLog("Sent response: " + response.left(64) + "...");
    }
    else if (message.startsWith("RESPONSE:") && isAlice) {
        QString response = message.mid(9);
        bool valid = protocolHandler->verifyResponse(response);
        QString ack = valid ? "ACK:1" : "ACK:0";
        networkManager->sendMessage(ack);
        appendLog("Received response: " + response.left(64) + "...");
        appendLog(QString("Verification: %1").arg(valid ? "SUCCESS" : "FAILED"));

        if (!valid) {
            appendLog("Authentication failed! Stopping protocol.");
            onStopProtocol();
        }
    }
    else if (message.startsWith("ACK:") && !isAlice) {
        bool ack = message.mid(4).toInt();
        appendLog(QString("Received acknowledgement: %1")
                      .arg(ack ? "SUCCESS" : "FAILED"));
        if (!ack) {
            appendLog("Authentication failed! Stopping protocol.");
            onStopProtocol();
        }
    }
}

void MainWindow::onStopProtocol()
{
    protocolRunning = false;
    protocolTimer->stop();

    if (networkManager) {
        networkManager->closeConnection();
        networkManager.reset();
    }

    ui->btnStartAlice->setEnabled(true);
    ui->btnStartBob->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->lblRole->setText("Role: Not Selected");

    appendLog("Protocol stopped");
}

void MainWindow::onConnectionLost()
{
    appendLog("Connection lost!");
    onStopProtocol();
}

void MainWindow::appendLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->txtLog->append("[" + timestamp + "] " + message);
}

void MainWindow::onClearLog()
{
    ui->txtLog->clear();
}
