QT += core network
QT -= gui

TARGET = SystemTemperatureSensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    SystemTemperatureSensorServer.h

SOURCES += \
    main.cpp \
    SystemTemperatureSensorServer.cpp

DISTFILES += \
    package/package-create.sh \
    package/package.control \
    package/package.init

INCLUDEPATH += \
    ../../GenericSensorServer

LIBS += \
    -L../../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer

target.path = /home/pi/SensorsCollector
INSTALLS += target

QMAKE_POST_LINK += cd $$_PRO_FILE_PWD_;
QMAKE_POST_LINK += $$_PRO_FILE_PWD_/package/package-create.sh;
