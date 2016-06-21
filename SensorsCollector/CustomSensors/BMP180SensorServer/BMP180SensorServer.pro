QT += core network
QT -= gui

TARGET = BMP180SensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    BMP180SensorServer.h

SOURCES += \
    main.cpp \
    BMP180SensorServer.cpp

DISTFILES += \
    package-create.sh \
    package.control \
    package.init

INCLUDEPATH += \
    ../GenericSensorServer

LIBS += \
    -L../../GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer \
    -L/usr/local/lib \
    -lwiringPi

target.path = /home/pi/SensorsCollector
INSTALLS += target

QMAKE_POST_LINK += cd $$_PRO_FILE_PWD_;
QMAKE_POST_LINK += $$_PRO_FILE_PWD_/package-create.sh;
