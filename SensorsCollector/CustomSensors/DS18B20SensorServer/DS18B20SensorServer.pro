QT += core network
QT -= gui

TARGET = DS18B20SensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    DS18B20SensorServer.cpp

HEADERS += \
    DS18B20SensorServer.h

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer

target.path = /home/pi/SensorsCollector
INSTALLS += target
