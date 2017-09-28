QT += core network sql
QT -= gui

TARGET = SensorsCollector

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS += \
    ApplicationLog.h \
    ConsoleApplication.h \
    Consts.h \
    SensorClient.h \
    SensorsCollector.h \
    Settings.h

SOURCES += \
    main.cpp \
    ApplicationLog.cpp \
    ConsoleApplication.cpp \
    SensorClient.cpp \
    SensorsCollector.cpp \
    Settings.cpp

DISTFILES += \
    package/package-create.sh \
    package/package.control \
    package/package.init \
    package/package.postinst

INCLUDEPATH += \
    ../SensorServers/GenericSensorServer

LIBS += \
    -L../../SensorServers/GenericSensorServer/GenericSensorServer-Release \
    -lGenericSensorServer

target.path = /home/pi/SensorsCollector
INSTALLS += target

QMAKE_POST_LINK += cd $$_PRO_FILE_PWD_;
QMAKE_POST_LINK += $$_PRO_FILE_PWD_/package/package-create.sh;
