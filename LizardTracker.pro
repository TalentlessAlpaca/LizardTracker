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
    ocvframe.cpp

HEADERS  += mainwindow.h \
    calibrationwindow.h \
    colorfilter.h \
    ocvframe.h \
    buffer.h

FORMS    += mainwindow.ui \
    calibrationwindow.ui

LIBS += `pkg-config opencv --libs`

RESOURCES += \
    Data/hallIcon.png \
    Data/defaultFilters.txt \
    Data/icon.ico \
    Data/file.png \
    Data/filter.png \
    Data/forward.png \
    Data/pause.png \
    Data/play.png \
    Data/plot.png \
    Data/property.png \
    Data/webcam.png \
    Data/delete.png \
    icons.qrc
    Data/add.png
    Data/clear.png
    Data/repeat.png
    Data/size.png
    Data/folder.png
    Data/save.png
    Data/copy.png

