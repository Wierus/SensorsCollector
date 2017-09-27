#include "VoltagePresenceSensorServer.h"

VoltagePresenceSensorServer::VoltagePresenceSensorServer() {
    if (wiringPiSetup() == 0) {
        qDebug("WiringPi initialized successfully");
    }
    else {
        qDebug("Error occurred while initializing WiringPi");
    }
}

double VoltagePresenceSensorServer::getCurrentValue(QString identifier) {
    bool identifierOk = false;
    int pin = identifier.toInt(&identifierOk);
    if (!identifierOk) {
        QString lastError = QString("Incorrect format identifier \"%1\".").arg(identifier);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    pinMode(pin, INPUT);
    return digitalRead(pin);
}
