QT += core network
QT -= gui

TARGET = ElectricPowerSensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    ElectricPowerSensorServer.cpp

HEADERS += \
    ElectricPowerSensorServer.h

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer \
    -L/usr/local/lib \
    -lwiringPi

target.path = /home/pi/SensorsCollector
INSTALLS += target
