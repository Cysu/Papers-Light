QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PapersLight
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += .

SOURCES += \
    main.cpp\
    gui/MainWindow.cpp \
    db/DatabaseHelper.cpp \
    common/Paper.cpp \
    gui/CategoryList.cpp \
    gui/PaperList.cpp \
    gui/SearchBar.cpp \
    gui/PaperInfoTable.cpp \
    common/CategoryStats.cpp \
    utils/utils.cpp \
    db/SearchHelper.cpp

HEADERS += \
    gui/MainWindow.h \
    db/DatabaseHelper.h \
    common/Paper.h \
    common/const.h \
    gui/CategoryList.h \
    gui/PaperList.h \
    gui/SearchBar.h \
    gui/PaperInfoTable.h \
    common/CategoryStats.h \
    utils/utils.h \
    db/SearchHelper.h
