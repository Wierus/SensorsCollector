#include "DHT22SensorServer.h"

DHT22SensorServer::DHT22SensorServer()  {
    if (wiringPiSetup() == 0) {
        qDebug("WiringPi initialized successfully");
    }
    else {
        qDebug("Error occurred while initializing WiringPi");
    }
}

DHT22SensorServer::ErrorCodes DHT22SensorServer::readDHT22(const int pin, double* humidity, double* temperature) {
    if (!this -> sendStartSignal(pin)) {
        return ResponseSignalNotDetected;
    }
    unsigned char data[DHT22_DATA_LENGTH] = {0};
    if (!this -> readData(pin, data)) {
        return TimeoutReadError;
    }
    unsigned char parity = 0;
    for (int i = 0; i < DHT22_DATA_LENGTH - 1; i++) {
        parity += data[i];
    }
    if (parity != data[DHT22_DATA_LENGTH - 1]) {
        return ParityReadError;
    }
    short humidityData    = (data[0] << 8) + data[1];
    short temperatureData = (data[2] << 8) + data[3];
    if (temperatureData & DHT22_TEMPERATURE_SIGN_MASK) {
        temperatureData &= ~DHT22_TEMPERATURE_SIGN_MASK;
        temperatureData = -temperatureData;
    }
    *humidity    = humidityData    * 0.1;
    *temperature = temperatureData * 0.1;
    return OperationOK;
}

bool DHT22SensorServer::sendStartSignal(const int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(DHT22_T_be);
    pinMode(pin, INPUT);
    delayMicroseconds(DHT22_T_go);
    if (!this -> waitLowHigh(pin)) {
        return false;
    }
    return true;
}

bool DHT22SensorServer::waitLowHigh(const int pin) {
    unsigned int timeout = millis() + DHT22_TIMEOUT;
    while (digitalRead(pin) == HIGH) {
        if (millis() > timeout) {
            return false;
        }
    }
    while (digitalRead(pin) == LOW) {
        if (millis() > timeout) {
            return false;
        }
    }
    return true;
}

bool DHT22SensorServer::readData(const int pin, unsigned char data[]) {
    pinMode(pin, INPUT);
    for (int i = 0; i < DHT22_DATA_LENGTH; i++) {
        if (!this -> readByte(pin, &data[i])) {
            return false;
        }
    }
    return true;
}

bool DHT22SensorServer::readByte(const int pin, unsigned char* byte) {
    for (int i = 0; i < 8; i++) {
        if (!this -> waitLowHigh(pin)) {
            return false;
        }
        delayMicroseconds(DHT22_T_H0);
        *byte <<= 1;
        if (digitalRead(pin) == HIGH) {
            *byte |= 1;
        }
    }
    return true;
}

double DHT22SensorServer::getCurrentValue(QString identifier) {
    bool identifierOk = false;
    int pin = identifier.toInt(&identifierOk);
    if (!identifierOk) {
        QString lastError = QString("Incorrect format identifier \"%1\".").arg(identifier);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    int attempt = 1;
    double humidity    = NAN;
    double temperature = NAN;
    while (attempt <= DHT22_MAX_FAILED_ATTEMPTS) {
        ErrorCodes result = this -> readDHT22(pin, &humidity, &temperature);
        switch (result) {
            case OperationOK: {
                qDebug("Attempt %i: OperationOK", attempt);
                return humidity;
            }
            case ResponseSignalNotDetected: {
                qDebug("Attempt %i: ResponseSignalNotDetected", attempt);
                break;
            }
            case TimeoutReadError: {
                qDebug("Attempt %i: TimeoutReadError", attempt);
                break;
            }
            case ParityReadError: {
                qDebug("Attempt %i: ParityReadError", attempt);
                break;
            }
        }
        attempt++;
        delay(DHT22_FAILED_REPEAT_INTERVAL);
    }
    QString lastError = QString("Maximum number of failed attempts (%1 attempts) exceeded for sensor \"%2\".").arg(DHT22_MAX_FAILED_ATTEMPTS).arg(identifier);
    this -> setLastError(identifier, lastError);
    qDebug(lastError.toLocal8Bit().data());
    return NAN;
}
