#include "SystemTemperatureSensorServer.h"

SystemTemperatureSensorServer::SystemTemperatureSensorServer() {
}

double SystemTemperatureSensorServer::getCurrentValue(QString identifier) {
    QFile file(SYSTEM_TEMPERATURE_FILE_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString lastError = QString("An error occurred while opening file \"%1\": (%2) %3.").arg(SYSTEM_TEMPERATURE_FILE_PATH).arg(file.error()).arg(file.errorString());
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    QTextStream stream(&file);
    QString line = stream.readLine();
    file.close();
    bool parseOk = false;
    double value = line.toDouble(&parseOk);
    if (!parseOk) {
        QString lastError = QString("An error occurred while parsing file \"%1\".").arg(SYSTEM_TEMPERATURE_FILE_PATH);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    return value * SYSTEM_TEMPERATURE_VALUE_MULTIPLIER;
}
