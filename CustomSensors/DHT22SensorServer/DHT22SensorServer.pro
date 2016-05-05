QT += core network
QT -= gui

TARGET = DHT22SensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    DHT22SensorServer.cpp

HEADERS += \
    DHT22SensorServer.h

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer \
    -L/usr/local/lib \
    -lwiringPi

target.path = /home/pi/SensorsCollector
INSTALLS += target
