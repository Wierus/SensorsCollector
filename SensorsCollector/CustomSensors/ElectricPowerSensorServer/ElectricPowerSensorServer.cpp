#include "ElectricPowerSensorServer.h"

int ElectricPowerSensorServer::interruptHandlersPinsArray[];

QMap<int, ElectricPowerSensorServer::SensorData>* ElectricPowerSensorServer::sensorsDataMap;

ElectricPowerSensorServer::ElectricPowerSensorServer() {
    if (wiringPiSetup() == 0) {
        qDebug("WiringPi initialized successfully");
    }
    else {
        qDebug("Error occurred while initializing WiringPi");
    }
    this -> sensorsDataMap = new QMap<int, SensorData>();
    for (int i = 0; i < ELECTRIC_POWER_SENSORS_MAX_COUNT; i++) {
        this -> interruptHandlersPinsArray[i] = -1;
    }
}

void ElectricPowerSensorServer::interruptHandler0() {
    interruptHandler(interruptHandlersPinsArray[0]);
}

void ElectricPowerSensorServer::interruptHandler1() {
    interruptHandler(interruptHandlersPinsArray[1]);
}

void ElectricPowerSensorServer::interruptHandler(const int pin) {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    // если уже был получен хотя бы один импульс со счетчика
    if (sensorsDataMap -> value(pin).impulsesCount > 0) {
        // интервал между текущим импульсом и самым ранним необработанным импульсом
        qint64 lastSpanMs = currentTime - sensorsDataMap -> operator[](pin).lastImpulseTime;
        if (lastSpanMs > ELECTRIC_POWER_IMPULSES_SPAN_IGNORE_TIME) {
            if (lastSpanMs <= ELECTRIC_POWER_IMPULSES_SPAN_WARNING_TIME) {
                qDebug() << "span: " << lastSpanMs << "WARNING";
            }
            sensorsDataMap -> operator[](pin).impulsesSpanMs += lastSpanMs;
        }
        else {
            // интервал между импульсами меньше минимально допустимого, текущий импульс принимается за дребезг и будет проигнорирован
            return;
        }
    }
    sensorsDataMap -> operator[](pin).lastImpulseTime = currentTime;
    sensorsDataMap -> operator[](pin).impulsesCount++;
}

double ElectricPowerSensorServer::getCurrentValue(QString identifier) {
    bool identifierOk = false;
    int pin = identifier.toInt(&identifierOk);
    if (!identifierOk) {
        QString lastError = QString("Incorrect format identifier \"%1\".").arg(identifier);
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    if (!this -> sensorsDataMap -> contains(pin)) {
        int interruptHandlerIndex = this -> sensorsDataMap -> count();
        switch (interruptHandlerIndex) {
            case 0: {
                wiringPiISR(pin, INT_EDGE_FALLING, &interruptHandler0);
                break;
            }
            case 1: {
                wiringPiISR(pin, INT_EDGE_FALLING, &interruptHandler1);
                break;
            }
            default: {
                QString lastError = QString("Can not create more handlers.");
                this -> setLastError(identifier, lastError);
                qDebug(lastError.toLocal8Bit().data());
                return NAN;
            }
        }
        this -> sensorsDataMap -> insert(pin, SensorData());
        interruptHandlersPinsArray[interruptHandlerIndex] = pin;
    }
    // еще не было получено достаточно импульсов со счетчика
    if (this -> sensorsDataMap -> value(pin).impulsesCount <= 1) {
        QString lastError = QString("Not enough impulses yet.");
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    double f = 1000.0 * (this -> sensorsDataMap -> value(pin).impulsesCount - 1) / this -> sensorsDataMap -> value(pin).impulsesSpanMs;
    this -> sensorsDataMap -> operator[](pin).impulsesCount = 1;
    this -> sensorsDataMap -> operator[](pin).impulsesSpanMs = 0;
    return f;
}
