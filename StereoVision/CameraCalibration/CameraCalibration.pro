#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:39:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

LIBS += "C:/opencv243/build/bin/*.dll"

INCLUDEPATH += "C:/opencv243/build/include"

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
