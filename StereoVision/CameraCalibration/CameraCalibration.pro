#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:39:02
#// PATH C:\MinGW\bin;C:\opencv243\build\bin;C:\Qt\4.8.4\bin; //
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

unix {
LIBS += -lopencv_calib3d -lopencv_contrib -lopencv_core     \
        -lopencv_features2d -lopencv_flann -lopencv_highgui \
        -lopencv_imgproc -lopencv_legacy -lopencv_ml        \
        -lopencv_objdetect \
        -lopencv_video # -lopencv_videostab -lopencv_ts lopencv_photo -lopencv_stitching
}

win32 {
    LIBS += "C:/opencv243/build/x86/mingw/bin/*.dll"
    INCLUDEPATH += "C:/opencv243/build/include"
}

win64 {
    LIBS += "C:/opencv243/build/bin/*.dll"
    INCLUDEPATH += "C:/opencv243/build/include"
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


