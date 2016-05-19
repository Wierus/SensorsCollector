#include "DS18B20SensorServer.h"

DS18B20SensorServer::DS18B20SensorServer() {
}

double DS18B20SensorServer::getCurrentValue(QString identifier) {
    QString filePath = QString(SENSOR_FILE_PATH_TEMPLATE).arg(identifier);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString lastError = QString("An error occurred while opening file \"%1\": (%2) %3.").arg(filePath).arg(file.error()).arg(file.errorString());
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    QTextStream stream(&file);
    QString line0 = stream.readLine();
    QString line1 = stream.readLine();
    file.close();
    if (line0.mid(SENSOR_LINE0_CRC_INDEX) != SENSOR_CRC_CORRECT) {
        QString lastError = QString("Incorrect checksum detected while parsing file \"%1\".").arg(filePath);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    bool parseOk = false;
    double value = line1.mid(SENSOR_LINE1_VALUE_INDEX).toDouble(&parseOk);
    if (!parseOk) {
        QString lastError = QString("An error occurred while parsing file \"%1\".").arg(filePath);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    return value * SENSOR_VALUE_MULTIPLIER;
}
