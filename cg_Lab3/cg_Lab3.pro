#-------------------------------------------------
#
# Project created by QtCreator 2015-04-05T14:07:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cg_Lab3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    linegraphicsview.cpp

HEADERS  += mainwindow.h \
    linegraphicsview.h \
    rasteralogithm.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

