/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frameOpen;
    QPushButton *cmdOpenCloseFeed;
    QLineEdit *txtPort;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *lblUPSpeed;
    QLabel *lblDOWNSpeed;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QFrame *frameMessage;
    QTextEdit *txtAnswer;
    QPushButton *cmdFile;
    QLabel *label;
    QLineEdit *txtMessage;
    QLineEdit *txtAddressIFace;
    QPushButton *cmdSend;
    QFrame *frameMemory;
    QLineEdit *txtAddressMemory;
    QTableWidget *tblMemory;
    QPushButton *cmdRead;
    QCheckBox *chkPollMem;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(673, 829);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frameOpen = new QFrame(centralWidget);
        frameOpen->setObjectName(QStringLiteral("frameOpen"));
        frameOpen->setGeometry(QRect(2, 0, 213, 31));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frameOpen->sizePolicy().hasHeightForWidth());
        frameOpen->setSizePolicy(sizePolicy);
        frameOpen->setMinimumSize(QSize(213, 31));
        frameOpen->setMaximumSize(QSize(213, 31));
        frameOpen->setFrameShape(QFrame::Box);
        frameOpen->setFrameShadow(QFrame::Raised);
        frameOpen->setLineWidth(2);
        frameOpen->setMidLineWidth(0);
        cmdOpenCloseFeed = new QPushButton(frameOpen);
        cmdOpenCloseFeed->setObjectName(QStringLiteral("cmdOpenCloseFeed"));
        cmdOpenCloseFeed->setGeometry(QRect(5, 5, 141, 23));
        cmdOpenCloseFeed->setAutoFillBackground(true);
        cmdOpenCloseFeed->setCheckable(true);
        cmdOpenCloseFeed->setChecked(false);
        txtPort = new QLineEdit(frameOpen);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setGeometry(QRect(148, 6, 61, 21));
        txtPort->setAlignment(Qt::AlignCenter);
        txtPort->setReadOnly(true);
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(356, 10, 311, 15));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lblUPSpeed = new QLabel(layoutWidget);
        lblUPSpeed->setObjectName(QStringLiteral("lblUPSpeed"));
        lblUPSpeed->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblUPSpeed);

        lblDOWNSpeed = new QLabel(layoutWidget);
        lblDOWNSpeed->setObjectName(QStringLiteral("lblDOWNSpeed"));
        lblDOWNSpeed->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblDOWNSpeed);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(2, 34, 669, 775));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frameMessage = new QFrame(layoutWidget1);
        frameMessage->setObjectName(QStringLiteral("frameMessage"));
        frameMessage->setFrameShape(QFrame::Box);
        frameMessage->setFrameShadow(QFrame::Raised);
        frameMessage->setLineWidth(2);
        frameMessage->setMidLineWidth(0);
        txtAnswer = new QTextEdit(frameMessage);
        txtAnswer->setObjectName(QStringLiteral("txtAnswer"));
        txtAnswer->setGeometry(QRect(5, 27, 661, 303));
        txtAnswer->setAutoFillBackground(true);
        txtAnswer->setReadOnly(true);
        txtAnswer->setTextInteractionFlags(Qt::NoTextInteraction);
        cmdFile = new QPushButton(frameMessage);
        cmdFile->setObjectName(QStringLiteral("cmdFile"));
        cmdFile->setEnabled(true);
        cmdFile->setGeometry(QRect(623, 4, 35, 21));
        sizePolicy.setHeightForWidth(cmdFile->sizePolicy().hasHeightForWidth());
        cmdFile->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        cmdFile->setFont(font);
        label = new QLabel(frameMessage);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(11, 4, 73, 16));
        label->setFrameShadow(QFrame::Sunken);
        txtMessage = new QLineEdit(frameMessage);
        txtMessage->setObjectName(QStringLiteral("txtMessage"));
        txtMessage->setGeometry(QRect(131, 5, 413, 20));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setBold(false);
        font1.setWeight(50);
        txtMessage->setFont(font1);
        txtMessage->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        txtMessage->setReadOnly(false);
        txtAddressIFace = new QLineEdit(frameMessage);
        txtAddressIFace->setObjectName(QStringLiteral("txtAddressIFace"));
        txtAddressIFace->setGeometry(QRect(84, 5, 47, 20));
        txtAddressIFace->setFont(font1);
        txtAddressIFace->setAlignment(Qt::AlignCenter);
        txtAddressIFace->setReadOnly(false);
        cmdSend = new QPushButton(frameMessage);
        cmdSend->setObjectName(QStringLiteral("cmdSend"));
        cmdSend->setGeometry(QRect(544, 4, 67, 21));
        cmdSend->setFont(font);

        verticalLayout->addWidget(frameMessage);

        frameMemory = new QFrame(layoutWidget1);
        frameMemory->setObjectName(QStringLiteral("frameMemory"));
        frameMemory->setFrameShape(QFrame::Box);
        frameMemory->setFrameShadow(QFrame::Raised);
        frameMemory->setLineWidth(2);
        frameMemory->setMidLineWidth(0);
        txtAddressMemory = new QLineEdit(frameMemory);
        txtAddressMemory->setObjectName(QStringLiteral("txtAddressMemory"));
        txtAddressMemory->setGeometry(QRect(5, 5, 67, 20));
        txtAddressMemory->setFont(font1);
        txtAddressMemory->setAlignment(Qt::AlignCenter);
        txtAddressMemory->setReadOnly(false);
        tblMemory = new QTableWidget(frameMemory);
        if (tblMemory->columnCount() < 17)
            tblMemory->setColumnCount(17);
        if (tblMemory->rowCount() < 32)
            tblMemory->setRowCount(32);
        tblMemory->setObjectName(QStringLiteral("tblMemory"));
        tblMemory->setGeometry(QRect(5, 26, 661, 411));
        tblMemory->setFont(font1);
        tblMemory->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tblMemory->setRowCount(32);
        tblMemory->setColumnCount(17);
        cmdRead = new QPushButton(frameMemory);
        cmdRead->setObjectName(QStringLiteral("cmdRead"));
        cmdRead->setGeometry(QRect(72, 3, 75, 23));
        cmdRead->setFont(font);
        chkPollMem = new QCheckBox(frameMemory);
        chkPollMem->setObjectName(QStringLiteral("chkPollMem"));
        chkPollMem->setGeometry(QRect(170, 7, 81, 16));

        verticalLayout->addWidget(frameMemory);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "USB Feed Driver test terminal", 0));
#ifndef QT_NO_ACCESSIBILITY
        frameOpen->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        frameOpen->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        cmdOpenCloseFeed->setText(QApplication::translate("MainWindow", "Find && Open USB Feed Port", 0));
#ifndef QT_NO_TOOLTIP
        txtPort->setToolTip(QApplication::translate("MainWindow", "Port Handle", 0));
#endif // QT_NO_TOOLTIP
        txtPort->setText(QApplication::translate("MainWindow", "0x0000", 0));
        lblUPSpeed->setText(QApplication::translate("MainWindow", "0", 0));
        lblDOWNSpeed->setText(QApplication::translate("MainWindow", "0", 0));
#ifndef QT_NO_ACCESSIBILITY
        frameMessage->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        frameMessage->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        cmdFile->setText(QApplication::translate("MainWindow", "File", 0));
        label->setText(QApplication::translate("MainWindow", "Send Message:", 0));
#ifndef QT_NO_TOOLTIP
        txtMessage->setToolTip(QApplication::translate("MainWindow", "Message for interface of device", 0));
#endif // QT_NO_TOOLTIP
        txtMessage->setText(QApplication::translate("MainWindow", "TEST", 0));
#ifndef QT_NO_TOOLTIP
        txtAddressIFace->setToolTip(QApplication::translate("MainWindow", "Address interface of device", 0));
#endif // QT_NO_TOOLTIP
        txtAddressIFace->setText(QApplication::translate("MainWindow", "DEV>", 0));
        cmdSend->setText(QApplication::translate("MainWindow", "Send", 0));
#ifndef QT_NO_ACCESSIBILITY
        frameMemory->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        frameMemory->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_TOOLTIP
        txtAddressMemory->setToolTip(QApplication::translate("MainWindow", "Address memory of device", 0));
#endif // QT_NO_TOOLTIP
        txtAddressMemory->setText(QApplication::translate("MainWindow", "0x20000000", 0));
#ifndef QT_NO_TOOLTIP
        cmdRead->setToolTip(QApplication::translate("MainWindow", "Read memory by address", 0));
#endif // QT_NO_TOOLTIP
        cmdRead->setText(QApplication::translate("MainWindow", "Read", 0));
        chkPollMem->setText(QApplication::translate("MainWindow", "AutoUpdate", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
