#-------------------------------------------------
#
# Project created by QtCreator 2016-10-12T09:23:29
#
#-------------------------------------------------

QT       += core gui network sql

#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chunil
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filethread.cpp \
    configdialog.cpp

HEADERS  += mainwindow.h \
    filethread.h \
    configdialog.h \
    define.h

FORMS    += mainwindow.ui \
    configdialog.ui

RESOURCES += \
    chunil.qrc
