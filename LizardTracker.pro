#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T14:07:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LizardTracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calibrationwindow.cpp \
    colorfilter.cpp

HEADERS  += mainwindow.h \
    calibrationwindow.h \
    colorfilter.h

FORMS    += mainwindow.ui \
    calibrationwindow.ui

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    ../../Pictures/awYeah.png \
    Data/defaultFilters.txt
