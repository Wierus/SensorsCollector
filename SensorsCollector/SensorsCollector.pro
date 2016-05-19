QT += core network sql
QT -= gui

TARGET = SensorsCollector

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS +=               \
    ApplicationLog.h     \
    ConsoleApplication.h \
    Consts.h             \
    SensorClient.h       \
    SensorsCollector.h   \
    Settings.h

SOURCES +=                 \
    main.cpp               \
    ApplicationLog.cpp     \
    ConsoleApplication.cpp \
    SensorClient.cpp       \
    SensorsCollector.cpp   \
    Settings.cpp

INCLUDEPATH += \
    CustomSensors/GenericSensorServer

LIBS += \
    -L$$_PRO_FILE_PWD_/CustomSensors/GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer

target.path = /home/pi/SensorsCollector
INSTALLS += target
