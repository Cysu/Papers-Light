QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PapersLight
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += .

SOURCES += \
    main.cpp\
    gui/MainWindow.cpp \
    db/DatabaseHelper.cpp \
    common/Paper.cpp

HEADERS += \
    gui/MainWindow.h \
    db/DatabaseHelper.h \
    common/Paper.h
