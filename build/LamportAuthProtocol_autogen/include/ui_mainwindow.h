/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblTitle;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnToggleTheme;
    QLabel *lblConfig;
    QLabel *lblRole;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnStartAlice;
    QPushButton *btnStartBob;
    QPushButton *btnStop;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QProgressBar *progressBar;
    QLabel *lblProgress;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QTextEdit *txtLog;
    QPushButton *btnClear;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(850, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lblTitle = new QLabel(centralwidget);
        lblTitle->setObjectName("lblTitle");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        lblTitle->setFont(font);
        lblTitle->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lblTitle);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        btnToggleTheme = new QPushButton(centralwidget);
        btnToggleTheme->setObjectName("btnToggleTheme");

        horizontalLayout_2->addWidget(btnToggleTheme);


        verticalLayout->addLayout(horizontalLayout_2);

        lblConfig = new QLabel(centralwidget);
        lblConfig->setObjectName("lblConfig");
        lblConfig->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblConfig);

        lblRole = new QLabel(centralwidget);
        lblRole->setObjectName("lblRole");
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        lblRole->setFont(font1);
        lblRole->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblRole);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnStartAlice = new QPushButton(centralwidget);
        btnStartAlice->setObjectName("btnStartAlice");

        horizontalLayout->addWidget(btnStartAlice);

        btnStartBob = new QPushButton(centralwidget);
        btnStartBob->setObjectName("btnStartBob");

        horizontalLayout->addWidget(btnStartBob);

        btnStop = new QPushButton(centralwidget);
        btnStop->setObjectName("btnStop");

        horizontalLayout->addWidget(btnStop);


        verticalLayout->addLayout(horizontalLayout);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        verticalLayout_2->addWidget(progressBar);

        lblProgress = new QLabel(groupBox);
        lblProgress->setObjectName("lblProgress");
        lblProgress->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(lblProgress);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        txtLog = new QTextEdit(groupBox_2);
        txtLog->setObjectName("txtLog");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(txtLog->sizePolicy().hasHeightForWidth());
        txtLog->setSizePolicy(sizePolicy);
        txtLog->setMinimumSize(QSize(0, 200));
        txtLog->setReadOnly(true);

        verticalLayout_3->addWidget(txtLog);

        btnClear = new QPushButton(groupBox_2);
        btnClear->setObjectName("btnClear");

        verticalLayout_3->addWidget(btnClear);


        verticalLayout->addWidget(groupBox_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 850, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Lamport's Hash Chain Authentication Protocol", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "Lamport's One-Way Hash Chain Authentication", nullptr));
        btnToggleTheme->setText(QCoreApplication::translate("MainWindow", "Toggle Theme", nullptr));
        lblConfig->setText(QCoreApplication::translate("MainWindow", "Configuration: Loading...", nullptr));
        lblRole->setText(QCoreApplication::translate("MainWindow", "Role: Not Selected", nullptr));
        btnStartAlice->setText(QCoreApplication::translate("MainWindow", "Start as Alice (Verifier)", nullptr));
        btnStartBob->setText(QCoreApplication::translate("MainWindow", "Start as Bob (Prover)", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "Stop Protocol", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Protocol Progress", nullptr));
        lblProgress->setText(QCoreApplication::translate("MainWindow", "Ready to start protocol", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Protocol Log", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear Log", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
