#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:39:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

LIBS += -lopencv_calib3d -lopencv_contrib -lopencv_core     \
        -lopencv_features2d -lopencv_flann -lopencv_highgui \
        -lopencv_imgproc -lopencv_legacy -lopencv_ml        \
        -lopencv_objdetect -lopencv_photo -lopencv_stitching\
        -lopencv_ts -lopencv_video -lopencv_videostab

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
