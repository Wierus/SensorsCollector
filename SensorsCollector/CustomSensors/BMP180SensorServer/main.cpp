#include <QCoreApplication>
#include <QTextCodec>

#include "BMP180SensorServer.h"

// Кодировка сообщений
#define TEXT_CODEC_NAME "UTF-8"

int main(int argc, char *argv[]) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TEXT_CODEC_NAME));
    if (argc <= 2) {
        qDebug("Not enough parameters.");
        qDebug("Usage:");
        qDebug("%s LocalServerName OwnerUserName", argv[0]);
        qDebug("LocalServerName - name of local server (name of file /tmp/<LocalServerName>)");
        qDebug("OwnerUserName   - owner of local server (owner of file /tmp/<LocalServerName>)");
        return -1;
    }
    QCoreApplication application(argc, argv);
    BMP180SensorServer sensor;
    if (!sensor.initializeLocalServer(argv[1], argv[2])) {
        qDebug("Program starting error.");
        return -1;
    }
    qDebug("Program started successfully.");
    return application.exec();
}
