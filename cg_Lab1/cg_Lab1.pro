#-------------------------------------------------
#
# Project created by QtCreator 2015-03-08T11:27:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cg_Lab1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colourmodel.cpp

HEADERS  += mainwindow.h \
    colourmodel.h \
    colourmodels.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    data.qrc

