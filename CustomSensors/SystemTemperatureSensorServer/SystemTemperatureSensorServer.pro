QT += core network
QT -= gui

TARGET = SystemTemperatureSensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    SystemTemperatureSensorServer.cpp

HEADERS += \
    SystemTemperatureSensorServer.h

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer

target.path = /home/pi/SensorsCollector
INSTALLS += target
