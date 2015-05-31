#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T00:40:47
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USBFeedDriverTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    thread.cpp \
    usbfeedport.cpp

HEADERS  += mainwindow.h \
    dwsfeed.h \
    thread.h \
    usbfeedport.h

FORMS    += mainwindow.ui
