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

    // Set window properties for better positioning
    this->resize(1000, 700);
    this->setMinimumSize(900, 600);

    // Add spacing
    ui->lblProgress->setContentsMargins(0, 5, 0, 0);
    ui->txtLog->setContentsMargins(8, 8, 8, 8);

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

    // Apply premium styling
    setDarkTheme();
    isDarkMode = true;

    // Connect theme toggle button
    connect(ui->btnToggleTheme, &QPushButton::clicked, this, &MainWindow::onToggleTheme);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onToggleTheme()
{
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        setDarkTheme();
    } else {
        setLightTheme();
    }
}

void MainWindow::setLightTheme()
{
    this->setStyleSheet(R"(
        /* Main Window Background with Gradient */
        QMainWindow {
            background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, 
                stop: 0 #f8fafc, 
                stop: 0.5 #f1f5f9, 
                stop: 1 #e2e8f0);
            color: #334155;
        }

        /* Central Widget */
        QWidget#centralwidget {
            background: transparent;
        }

        /* Title Label */
        QLabel#lblTitle {
            background: rgba(255, 255, 255, 0.8);
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 8px;
            padding: 12px 16px;
            font-family: 'Segoe UI', sans-serif;
            font-size: 16px;
            font-weight: 600;
            color: #334155;
            margin: 6px;
        }

        /* Configuration and Role Labels */
        QLabel#lblConfig, QLabel#lblRole {
            background: rgba(255, 255, 255, 0.6);
            border: 1px solid rgba(148, 163, 184, 0.15);
            border-radius: 6px;
            padding: 6px 12px;
            font-size: 12px;
            color: #64748b;
            margin: 3px 6px;
            font-family: 'Segoe UI', sans-serif;
        }

        QLabel#lblRole {
            font-weight: 500;
            font-size: 13px;
            color: #475569;
        }

        /* Buttons */
        QPushButton {
            background: rgba(255, 255, 255, 0.9);
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 6px;
            color: #475569;
            font-size: 12px;
            font-weight: 500;
            padding: 8px 16px;
            margin: 3px;
            min-height: 16px;
            font-family: 'Segoe UI', sans-serif;
        }

        QPushButton:hover {
            background: rgba(248, 250, 252, 0.95);
            border: 1px solid rgba(148, 163, 184, 0.3);
        }

        QPushButton:pressed {
            background: rgba(241, 245, 249, 0.9);
        }

        /* Primary Action Buttons */
        QPushButton#btnStartAlice, QPushButton#btnStartBob {
            background: rgba(59, 130, 246, 0.9);
            border: 1px solid rgba(37, 99, 235, 0.3);
            color: #ffffff;
            font-weight: 600;
        }

        QPushButton#btnStartAlice:hover, QPushButton#btnStartBob:hover {
            background: rgba(37, 99, 235, 0.95);
        }

        /* Stop Button */
        QPushButton#btnStop {
            background: rgba(239, 68, 68, 0.9);
            border: 1px solid rgba(220, 38, 38, 0.3);
            color: #ffffff;
            font-weight: 600;
        }

        QPushButton#btnStop:hover {
            background: rgba(220, 38, 38, 0.95);
        }

        /* Theme Toggle Button */
        QPushButton#btnToggleTheme {
            background: rgba(107, 114, 128, 0.9);
            border: 1px solid rgba(75, 85, 99, 0.3);
            color: #ffffff;
            font-weight: 500;
            border-radius: 12px;
            padding: 6px 12px;
            font-size: 11px;
        }

        QPushButton#btnToggleTheme:hover {
            background: rgba(75, 85, 99, 0.95);
        }

        /* Group Boxes */
        QGroupBox {
            font-weight: 600;
            font-size: 13px;
            color: #475569;
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 8px;
            margin: 8px 6px;
            padding-top: 16px;
            background: rgba(255, 255, 255, 0.5);
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 12px;
            background: rgba(255, 255, 255, 0.9);
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 4px;
            color: #475569;
            margin-left: 8px;
            top: -8px;
            font-size: 12px;
        }

        /* Progress Bar */
        QProgressBar {
            background: rgba(241, 245, 249, 0.8);
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 6px;
            height: 10px;
            text-align: center;
            margin: 6px;
            font-size: 10px;
            color: #475569;
        }

        QProgressBar::chunk {
            background: #3b82f6;
            border-radius: 5px;
        }

        /* Progress Label */
        QLabel#lblProgress {
            background: rgba(255, 255, 255, 0.6);
            border: 1px solid rgba(148, 163, 184, 0.15);
            border-radius: 4px;
            color: #64748b;
            font-size: 11px;
            font-weight: 500;
            margin: 4px 6px;
            padding: 4px 8px;
            text-align: center;
        }

        /* Text Edit (Log) */
        QTextEdit#txtLog {
            background: rgba(248, 250, 252, 0.9);
            border: 1px solid rgba(148, 163, 184, 0.2);
            border-radius: 8px;
            color: #334155;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 11px;
            padding: 8px;
            margin: 6px;
            selection-background-color: rgba(59, 130, 246, 0.2);
        }

        /* Scrollbar */
        QTextEdit QScrollBar:vertical {
            background: rgba(241, 245, 249, 0.6);
            width: 8px;
            border-radius: 4px;
        }

        QTextEdit QScrollBar::handle:vertical {
            background: rgba(148, 163, 184, 0.5);
            border-radius: 4px;
            min-height: 20px;
        }

        QTextEdit QScrollBar::handle:vertical:hover {
            background: rgba(100, 116, 139, 0.7);
        }

        /* Clear Button */
        QPushButton#btnClear {
            background: rgba(156, 163, 175, 0.8);
            border: 1px solid rgba(107, 114, 128, 0.2);
            color: #4b5563;
            font-size: 11px;
            padding: 6px 12px;
            margin: 6px;
        }

        QPushButton#btnClear:hover {
            background: rgba(107, 114, 128, 0.9);
            color: #ffffff;
        }
    )");
}

void MainWindow::setDarkTheme() {
    this->setStyleSheet(R"(
        /* Main Window Background with Gradient */
        QMainWindow {
            background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, 
                stop: 0 #1e293b, 
                stop: 0.5 #334155, 
                stop: 1 #475569);
            color: #e2e8f0;
        }

        /* Central Widget */
        QWidget#centralwidget {
            background: transparent;
        }

        /* Title Label */
        QLabel#lblTitle {
            background: rgba(51, 65, 85, 0.8);
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 8px;
            padding: 12px 16px;
            font-family: 'Segoe UI', sans-serif;
            font-size: 16px;
            font-weight: 600;
            color: #f1f5f9;
            margin: 6px;
        }

        /* Configuration and Role Labels */
        QLabel#lblConfig, QLabel#lblRole {
            background: rgba(30, 41, 59, 0.6);
            border: 1px solid rgba(71, 85, 105, 0.2);
            border-radius: 6px;
            padding: 6px 12px;
            font-size: 12px;
            color: #cbd5e1;
            margin: 3px 6px;
            font-family: 'Segoe UI', sans-serif;
        }

        QLabel#lblRole {
            font-weight: 500;
            font-size: 13px;
            color: #e2e8f0;
        }

        /* Buttons */
        QPushButton {
            background: rgba(51, 65, 85, 0.9);
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 6px;
            color: #e2e8f0;
            font-size: 12px;
            font-weight: 500;
            padding: 8px 16px;
            margin: 3px;
            min-height: 16px;
            font-family: 'Segoe UI', sans-serif;
        }

        QPushButton:hover {
            background: rgba(71, 85, 105, 0.95);
            border: 1px solid rgba(100, 116, 139, 0.4);
        }

        QPushButton:pressed {
            background: rgba(30, 41, 59, 0.9);
        }

        /* Primary Action Buttons */
        QPushButton#btnStartAlice, QPushButton#btnStartBob {
            background: rgba(59, 130, 246, 0.9);
            border: 1px solid rgba(37, 99, 235, 0.4);
            color: #ffffff;
            font-weight: 600;
        }

        QPushButton#btnStartAlice:hover, QPushButton#btnStartBob:hover {
            background: rgba(37, 99, 235, 0.95);
        }

        /* Stop Button */
        QPushButton#btnStop {
            background: rgba(239, 68, 68, 0.9);
            border: 1px solid rgba(220, 38, 38, 0.4);
            color: #ffffff;
            font-weight: 600;
        }

        QPushButton#btnStop:hover {
            background: rgba(220, 38, 38, 0.95);
        }

        /* Theme Toggle Button */
        QPushButton#btnToggleTheme {
            background: rgba(71, 85, 105, 0.9);
            border: 1px solid rgba(100, 116, 139, 0.3);
            color: #e2e8f0;
            font-weight: 500;
            border-radius: 12px;
            padding: 6px 12px;
            font-size: 11px;
        }

        QPushButton#btnToggleTheme:hover {
            background: rgba(100, 116, 139, 0.95);
        }

        /* Group Boxes */
        QGroupBox {
            font-weight: 600;
            font-size: 13px;
            color: #e2e8f0;
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 8px;
            margin: 8px 6px;
            padding-top: 16px;
            background: rgba(30, 41, 59, 0.5);
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 4px 12px;
            background: rgba(51, 65, 85, 0.9);
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 4px;
            color: #f1f5f9;
            margin-left: 8px;
            top: -8px;
            font-size: 12px;
        }

        /* Progress Bar */
        QProgressBar {
            background: rgba(30, 41, 59, 0.8);
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 6px;
            height: 10px;
            text-align: center;
            margin: 6px;
            font-size: 10px;
            color: #e2e8f0;
        }

        QProgressBar::chunk {
            background: #3b82f6;
            border-radius: 5px;
        }

        /* Progress Label */
        QLabel#lblProgress {
            background: rgba(30, 41, 59, 0.6);
            border: 1px solid rgba(71, 85, 105, 0.2);
            border-radius: 4px;
            color: #cbd5e1;
            font-size: 11px;
            font-weight: 500;
            margin: 4px 6px;
            padding: 4px 8px;
            text-align: center;
        }

        /* Text Edit (Log) */
        QTextEdit#txtLog {
            background: rgba(15, 23, 42, 0.9);
            border: 1px solid rgba(71, 85, 105, 0.3);
            border-radius: 8px;
            color: #e2e8f0;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 11px;
            padding: 8px;
            margin: 6px;
            selection-background-color: rgba(59, 130, 246, 0.3);
        }

        /* Scrollbar */
        QTextEdit QScrollBar:vertical {
            background: rgba(30, 41, 59, 0.6);
            width: 8px;
            border-radius: 4px;
        }

        QTextEdit QScrollBar::handle:vertical {
            background: rgba(71, 85, 105, 0.7);
            border-radius: 4px;
            min-height: 20px;
        }

        QTextEdit QScrollBar::handle:vertical:hover {
            background: rgba(100, 116, 139, 0.8);
        }

        /* Clear Button */
        QPushButton#btnClear {
            background: rgba(71, 85, 105, 0.8);
            border: 1px solid rgba(100, 116, 139, 0.3);
            color: #cbd5e1;
            font-size: 11px;
            padding: 6px 12px;
            margin: 6px;
        }

        QPushButton#btnClear:hover {
            background: rgba(100, 116, 139, 0.9);
            color: #f1f5f9;
        }
    )");
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
    QString formattedMessage = "[" + timestamp + "] " + message;

    if (message.contains("SUCCESS")) {
        formattedMessage.replace("SUCCESS", "<font color=\"#10b981\">SUCCESS</font>");
    } else if (message.contains("FAILED")) {
        formattedMessage.replace("FAILED", "<font color=\"#ef4444\">FAILED</font>");
    } else if (message.contains("Connection established") || message.contains("Protocol completed")) {
        formattedMessage = "<font color=\"#3b82f6\">" + formattedMessage + "</font>";
    } else if (message.contains("challenge") || message.contains("response")) {
        formattedMessage = "<font color=\"#8b5cf6\">" + formattedMessage + "</font>";
    }

    ui->txtLog->append(formattedMessage);
}

void MainWindow::onClearLog()
{
    ui->txtLog->clear();
}