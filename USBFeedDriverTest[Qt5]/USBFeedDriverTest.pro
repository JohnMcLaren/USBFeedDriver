#-------------------------------------------------
#
# Project created by QtCreator 2015-01-22T00:10:44
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USBFeedDriverTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usbfeedport.cpp \
    thread.cpp

HEADERS  += mainwindow.h \
    usbfeedport.h \
    dwsfeed.h \
    thread.h

FORMS    += mainwindow.ui
