QT += core network
QT -= gui

TARGET = VoltagePresenceSensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    VoltagePresenceSensorServer.cpp

HEADERS += \
    VoltagePresenceSensorServer.h

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer \
    -L/usr/local/lib \
    -lwiringPi

target.path = /home/pi/SensorsCollector
INSTALLS += target
