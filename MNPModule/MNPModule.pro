
QT       += core gui

TARGET = MNPModule
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    filehandler.cpp \
    worker.cpp

RESOURCES += \
    res.qrc

HEADERS += \
    filehandler.h \
    worker.h
