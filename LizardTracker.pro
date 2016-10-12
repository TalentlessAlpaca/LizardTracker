#-------------------------------------------------
#
# Project created by QtCreator 2016-09-28T14:07:55
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = LizardTracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    calibrationwindow.cpp \
    colorfilter.cpp \
    ocvframe.cpp \
    mouse.cpp

HEADERS  += mainwindow.h \
    calibrationwindow.h \
    colorfilter.h \
    ocvframe.h \
    mouse.h

FORMS    += mainwindow.ui \
    calibrationwindow.ui

LIBS += `pkg-config opencv --libs`

DISTFILES += \
    Data/defaultFilters.txt \
    Data/icon.ico \

