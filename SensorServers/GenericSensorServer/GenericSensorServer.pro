QT += core network
QT -= gui

TARGET = GenericSensorServer

TEMPLATE = lib

CONFIG += staticlib

HEADERS += \
    GenericSensorServer.h

SOURCES += \
    GenericSensorServer.cpp
