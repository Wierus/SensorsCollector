QT += core network
QT -= gui

TARGET = ElectricPowerSensorServer

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ElectricPowerSensorServer.h

SOURCES += \
    main.cpp \
    ElectricPowerSensorServer.cpp

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
