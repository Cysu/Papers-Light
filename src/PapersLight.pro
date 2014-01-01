#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T19:01:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PapersLight
TEMPLATE = app


SOURCES += \
    main.cpp\
    gui/MainWindow.cpp \
    db/DatabaseHelper.cpp \
    common/Paper.cpp

HEADERS += \
    gui/MainWindow.h \
    db/DatabaseHelper.h \
    common/Paper.h
