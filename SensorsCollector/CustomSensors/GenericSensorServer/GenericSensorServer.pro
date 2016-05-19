QT += core network
QT -= gui

TARGET = GenericSensorServer

TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    GenericSensorServer.cpp

HEADERS += \
    GenericSensorServer.h
