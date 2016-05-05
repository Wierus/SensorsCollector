#include "SensorClient.h"

SensorClient::SensorClient(ApplicationLog* log, unsigned int id, QString type, QString serverName, QString identifier, unsigned int refreshInterval) {
    this -> log = log;
    this -> id = id;
    this -> type = type;
    this -> serverName = serverName;
    this -> identifier = identifier;
    this -> refreshInterval = refreshInterval;
    this -> localClient = new QLocalSocket();
    connect(this -> localClient, SIGNAL(disconnected()), this, SLOT(on_localClient_disconnected()));
    connect(this -> localClient, SIGNAL(readyRead()),    this, SLOT(on_localClient_readyRead()));
    this -> refreshTimer = new QTimer();
    this -> refreshTimer -> setInterval(this -> refreshInterval * 1000);
    connect(this -> refreshTimer, SIGNAL(timeout()), this, SLOT(on_refreshTimer_timeout()));
}

unsigned int SensorClient::getId() {
    return this -> id;
}

QString SensorClient::getType() {
    return this -> type;
}

QString SensorClient::getServerName() {
    return this -> serverName;
}

QString SensorClient::getIdentifier() {
    return this -> identifier;
}

unsigned int SensorClient::getRefreshInterval() {
    return this -> refreshInterval;
}

bool SensorClient::connectToServer() {
    if (this -> localClient -> state() == QLocalSocket::ConnectedState) {
        return true;
    }
    this -> localClient -> connectToServer(this -> serverName);
    if (!this -> localClient -> waitForConnected(SENSOR_CLIENT_WAIT_FOR_CONNECTED_TIMEOUT)) {
        if (this -> log != NULL) {
            this -> log -> write(QString("Sensor %1: an error occurred while connecting to the local server \"%2\": \"%3\".").arg(this -> toString()).arg(this -> serverName).arg(this -> localClient -> errorString()));
        }
        return false;
    }
    if (this -> log != NULL) {
        this -> log -> write(QString("Sensor %1: connection to the local server \"%2\" established successfully.").arg(this -> toString()).arg(this -> serverName));
    }
    return true;
}

void SensorClient::startCollect() {
    this -> sendGetRequest();
    this -> refreshTimer -> start();
}

void SensorClient::sendGetRequest() {
    if (this -> localClient -> state() != QLocalSocket::ConnectedState) {
        if (this -> log != NULL) {
            this -> log -> write(QString("Sensor %1: no connection to the local server \"%2\", connection attempt will be performed.").arg(this -> toString()).arg(this -> serverName));
        }
    }
    if (!this -> connectToServer()) {
        return;
    }
    QDataStream stream(this -> localClient);
    stream << (quint8)GenericSensorServer::CommandTypeGetValue << this -> identifier;
    if (!this -> localClient -> waitForReadyRead(SENSOR_CLIENT_WAIT_FOR_READY_READ_TIMEOUT)) {
        if (this -> log != NULL) {
            this -> log -> write(QString("Sensor %1: data from the local server \"%2\" is not received within the timeout period (%3 ms).").arg(this -> toString()).arg(this -> serverName).arg(SENSOR_CLIENT_WAIT_FOR_READY_READ_TIMEOUT));
        }
    }
}

void SensorClient::on_refreshTimer_timeout() {
    this -> sendGetRequest();
}

void SensorClient::on_localClient_disconnected() {
    this -> log -> write(QString("Sensor %1: connection to the local server \"%2\" lost.").arg(this -> toString()).arg(this -> serverName));
}

void SensorClient::on_localClient_readyRead() {
    QDataStream stream(this -> localClient);
    this -> parseResponse(&stream);
}

void SensorClient::parseResponse(QDataStream* inputStream) {
    quint8 commandType = 0;
    *inputStream >> commandType;
    switch (commandType) {
        case GenericSensorServer::CommandTypeOk: {
            double value;
            QDateTime timestamp;
            *inputStream >> value >> timestamp;
            qDebug("<- Ok %f %s", value, timestamp.toString("yyyy.MM.dd hh:mm:ss.zzz").toLocal8Bit().data());
            emit this -> valueRefreshSuccessed(value, timestamp);
            break;
        }
        case GenericSensorServer::CommandTypeError: {
            QString lastError;
            *inputStream >> lastError;
            qDebug("<- Error %s", lastError.toLocal8Bit().data());
            emit this -> valueRefreshFailed(QString("Error response received from the server: \"%1\"").arg(lastError));
            break;
        }
        default: {
            qDebug("<- Unknown command type %i", commandType);
            emit this -> valueRefreshFailed(QString("Unknown response received from the server: \"%1\".").arg(commandType));
            break;
        }
    }
}

QString SensorClient::toString() {
    return QString("{Id = \"%1\"; Type = \"%2\"; Identifier = \"%3\"; RefreshInterval = \"%4\"}").arg(this -> id).arg(this -> type).arg(this -> identifier).arg(this -> refreshInterval);
}
