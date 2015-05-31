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
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QFrame *frameMessage;
    QTextEdit *txtAnswer;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chkRW;
    QLineEdit *txtAddressIFace;
    QLineEdit *txtMessage;
    QPushButton *cmdSend;
    QPushButton *cmdFile;
    QFrame *frameMemory;
    QTableWidget *tblMemory;
    QPushButton *cmdRead;
    QLineEdit *txtAddressMemory;
    QProgressBar *barProgressTXfer;
    QCheckBox *chkPollMem;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QFrame *frameOpen;
    QPushButton *cmdOpenCloseFeed;
    QLineEdit *txtPort;
    QCheckBox *chkFeedInt;
    QSpacerItem *horizontalSpacer_2;
    QLabel *lblUPSpeed;
    QLabel *lblDOWNSpeed;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(693, 615);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Sans Serif"));
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frameMessage = new QFrame(centralWidget);
        frameMessage->setObjectName(QStringLiteral("frameMessage"));
        frameMessage->setGeometry(QRect(6, 38, 681, 201));
        frameMessage->setFrameShape(QFrame::Panel);
        frameMessage->setFrameShadow(QFrame::Raised);
        frameMessage->setLineWidth(2);
        frameMessage->setMidLineWidth(0);
        txtAnswer = new QTextEdit(frameMessage);
        txtAnswer->setObjectName(QStringLiteral("txtAnswer"));
        txtAnswer->setGeometry(QRect(5, 27, 671, 170));
        QFont font1;
        font1.setFamily(QStringLiteral("Sans Serif"));
        font1.setPointSize(8);
        txtAnswer->setFont(font1);
        txtAnswer->setAutoFillBackground(true);
        txtAnswer->setReadOnly(true);
        txtAnswer->setTextInteractionFlags(Qt::NoTextInteraction);
        layoutWidget = new QWidget(frameMessage);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 5, 681, 22));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, 0, 5, 0);
        chkRW = new QCheckBox(layoutWidget);
        chkRW->setObjectName(QStringLiteral("chkRW"));
        QFont font2;
        font2.setPointSize(7);
        font2.setItalic(true);
        chkRW->setFont(font2);
        chkRW->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_2->addWidget(chkRW);

        txtAddressIFace = new QLineEdit(layoutWidget);
        txtAddressIFace->setObjectName(QStringLiteral("txtAddressIFace"));
        txtAddressIFace->setMaximumSize(QSize(60, 20));
        QFont font3;
        font3.setFamily(QStringLiteral("Sans Serif"));
        font3.setPointSize(8);
        font3.setBold(false);
        font3.setWeight(50);
        txtAddressIFace->setFont(font3);
        txtAddressIFace->setAlignment(Qt::AlignCenter);
        txtAddressIFace->setReadOnly(false);

        horizontalLayout_2->addWidget(txtAddressIFace);

        txtMessage = new QLineEdit(layoutWidget);
        txtMessage->setObjectName(QStringLiteral("txtMessage"));
        txtMessage->setMaximumSize(QSize(16777215, 20));
        txtMessage->setFont(font3);
        txtMessage->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        txtMessage->setReadOnly(false);

        horizontalLayout_2->addWidget(txtMessage);

        cmdSend = new QPushButton(layoutWidget);
        cmdSend->setObjectName(QStringLiteral("cmdSend"));
        cmdSend->setMaximumSize(QSize(80, 20));
        cmdSend->setFont(font3);

        horizontalLayout_2->addWidget(cmdSend);

        cmdFile = new QPushButton(layoutWidget);
        cmdFile->setObjectName(QStringLiteral("cmdFile"));
        cmdFile->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmdFile->sizePolicy().hasHeightForWidth());
        cmdFile->setSizePolicy(sizePolicy1);
        cmdFile->setMaximumSize(QSize(40, 20));
        cmdFile->setFont(font3);

        horizontalLayout_2->addWidget(cmdFile);

        frameMemory = new QFrame(centralWidget);
        frameMemory->setObjectName(QStringLiteral("frameMemory"));
        frameMemory->setGeometry(QRect(6, 240, 681, 351));
        frameMemory->setFrameShape(QFrame::Panel);
        frameMemory->setFrameShadow(QFrame::Raised);
        tblMemory = new QTableWidget(frameMemory);
        if (tblMemory->columnCount() < 17)
            tblMemory->setColumnCount(17);
        if (tblMemory->rowCount() < 32)
            tblMemory->setRowCount(32);
        tblMemory->setObjectName(QStringLiteral("tblMemory"));
        tblMemory->setGeometry(QRect(5, 25, 671, 325));
        tblMemory->setFont(font3);
        tblMemory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblMemory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tblMemory->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tblMemory->setRowCount(32);
        tblMemory->setColumnCount(17);
        cmdRead = new QPushButton(frameMemory);
        cmdRead->setObjectName(QStringLiteral("cmdRead"));
        cmdRead->setGeometry(QRect(87, 3, 80, 20));
        sizePolicy.setHeightForWidth(cmdRead->sizePolicy().hasHeightForWidth());
        cmdRead->setSizePolicy(sizePolicy);
        cmdRead->setMaximumSize(QSize(80, 20));
        cmdRead->setFont(font3);
        txtAddressMemory = new QLineEdit(frameMemory);
        txtAddressMemory->setObjectName(QStringLiteral("txtAddressMemory"));
        txtAddressMemory->setGeometry(QRect(5, 3, 82, 20));
        sizePolicy.setHeightForWidth(txtAddressMemory->sizePolicy().hasHeightForWidth());
        txtAddressMemory->setSizePolicy(sizePolicy);
        txtAddressMemory->setMaximumSize(QSize(90, 20));
        txtAddressMemory->setFont(font3);
        txtAddressMemory->setAlignment(Qt::AlignCenter);
        txtAddressMemory->setReadOnly(false);
        barProgressTXfer = new QProgressBar(frameMemory);
        barProgressTXfer->setObjectName(QStringLiteral("barProgressTXfer"));
        barProgressTXfer->setEnabled(true);
        barProgressTXfer->setGeometry(QRect(170, 8, 417, 10));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(barProgressTXfer->sizePolicy().hasHeightForWidth());
        barProgressTXfer->setSizePolicy(sizePolicy2);
        barProgressTXfer->setMaximumSize(QSize(16777215, 10));
        barProgressTXfer->setAutoFillBackground(true);
        barProgressTXfer->setStyleSheet(QLatin1String("QProgressBar\n"
"{\n"
"	color: rgb(0, 0, 0);\n"
"	text-align: center;\n"
"}\n"
"\n"
"QProgressBar::chunk\n"
"{\n"
"	width: 10px;\n"
"	margin: 0.5px;\n"
"	background-color: rgb(0, 194, 7);\n"
"}"));
        barProgressTXfer->setValue(50);
        barProgressTXfer->setTextVisible(false);
        chkPollMem = new QCheckBox(frameMemory);
        chkPollMem->setObjectName(QStringLiteral("chkPollMem"));
        chkPollMem->setGeometry(QRect(586, 3, 90, 20));
        sizePolicy.setHeightForWidth(chkPollMem->sizePolicy().hasHeightForWidth());
        chkPollMem->setSizePolicy(sizePolicy);
        chkPollMem->setMaximumSize(QSize(90, 20));
        chkPollMem->setFont(font1);
        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(5, 3, 671, 33));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frameOpen = new QFrame(layoutWidget1);
        frameOpen->setObjectName(QStringLiteral("frameOpen"));
        sizePolicy1.setHeightForWidth(frameOpen->sizePolicy().hasHeightForWidth());
        frameOpen->setSizePolicy(sizePolicy1);
        frameOpen->setMinimumSize(QSize(213, 31));
        frameOpen->setMaximumSize(QSize(213, 31));
        frameOpen->setFrameShape(QFrame::Box);
        frameOpen->setFrameShadow(QFrame::Raised);
        frameOpen->setLineWidth(2);
        frameOpen->setMidLineWidth(0);
        cmdOpenCloseFeed = new QPushButton(frameOpen);
        cmdOpenCloseFeed->setObjectName(QStringLiteral("cmdOpenCloseFeed"));
        cmdOpenCloseFeed->setGeometry(QRect(5, 4, 141, 22));
        QFont font4;
        font4.setFamily(QStringLiteral("Sans Serif"));
        font4.setPointSize(7);
        cmdOpenCloseFeed->setFont(font4);
        cmdOpenCloseFeed->setAutoFillBackground(true);
        cmdOpenCloseFeed->setCheckable(true);
        cmdOpenCloseFeed->setChecked(false);
        txtPort = new QLineEdit(frameOpen);
        txtPort->setObjectName(QStringLiteral("txtPort"));
        txtPort->setGeometry(QRect(146, 5, 61, 21));
        txtPort->setFont(font1);
        txtPort->setAlignment(Qt::AlignCenter);
        txtPort->setReadOnly(true);

        horizontalLayout->addWidget(frameOpen);

        chkFeedInt = new QCheckBox(layoutWidget1);
        chkFeedInt->setObjectName(QStringLiteral("chkFeedInt"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(chkFeedInt->sizePolicy().hasHeightForWidth());
        chkFeedInt->setSizePolicy(sizePolicy3);
        chkFeedInt->setMaximumSize(QSize(16777215, 20));
        chkFeedInt->setFont(font1);
        chkFeedInt->setChecked(false);

        horizontalLayout->addWidget(chkFeedInt);

        horizontalSpacer_2 = new QSpacerItem(18, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        lblUPSpeed = new QLabel(layoutWidget1);
        lblUPSpeed->setObjectName(QStringLiteral("lblUPSpeed"));
        lblUPSpeed->setMinimumSize(QSize(100, 0));
        lblUPSpeed->setMaximumSize(QSize(16777215, 20));
        lblUPSpeed->setFont(font1);
        lblUPSpeed->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblUPSpeed);

        lblDOWNSpeed = new QLabel(layoutWidget1);
        lblDOWNSpeed->setObjectName(QStringLiteral("lblDOWNSpeed"));
        lblDOWNSpeed->setMinimumSize(QSize(100, 0));
        lblDOWNSpeed->setMaximumSize(QSize(16777215, 20));
        lblDOWNSpeed->setFont(font1);
        lblDOWNSpeed->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblDOWNSpeed);

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
        frameMessage->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        frameMessage->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        chkRW->setText(QApplication::translate("MainWindow", "Read / Write", 0));
#ifndef QT_NO_TOOLTIP
        txtAddressIFace->setToolTip(QApplication::translate("MainWindow", "Address interface of device", 0));
#endif // QT_NO_TOOLTIP
        txtAddressIFace->setText(QApplication::translate("MainWindow", "DEV>", 0));
#ifndef QT_NO_TOOLTIP
        txtMessage->setToolTip(QApplication::translate("MainWindow", "Message for interface of device", 0));
#endif // QT_NO_TOOLTIP
        txtMessage->setText(QApplication::translate("MainWindow", "TEST", 0));
        cmdSend->setText(QApplication::translate("MainWindow", "Send", 0));
        cmdFile->setText(QApplication::translate("MainWindow", "File", 0));
#ifndef QT_NO_ACCESSIBILITY
        frameMemory->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        frameMemory->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_TOOLTIP
        cmdRead->setToolTip(QApplication::translate("MainWindow", "Read memory by address", 0));
#endif // QT_NO_TOOLTIP
        cmdRead->setText(QApplication::translate("MainWindow", "Read", 0));
#ifndef QT_NO_TOOLTIP
        txtAddressMemory->setToolTip(QApplication::translate("MainWindow", "Address memory of device", 0));
#endif // QT_NO_TOOLTIP
        txtAddressMemory->setText(QApplication::translate("MainWindow", "0x20000000", 0));
        barProgressTXfer->setFormat(QApplication::translate("MainWindow", "%p", 0));
        chkPollMem->setText(QApplication::translate("MainWindow", "AutoUpdate", 0));
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
        chkFeedInt->setText(QApplication::translate("MainWindow", "Feed Interrupt", 0));
        lblUPSpeed->setText(QApplication::translate("MainWindow", "0", 0));
        lblDOWNSpeed->setText(QApplication::translate("MainWindow", "0", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
