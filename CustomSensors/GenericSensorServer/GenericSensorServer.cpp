#include "GenericSensorServer.h"

GenericSensorServer::GenericSensorServer() {
    this -> clientsList = new QList<QLocalSocket*>();
    this -> sensorErrorsMap = new QMap<QString, QString>();
}

bool GenericSensorServer::initializeLocalServer(QString localServerName, QString ownerUserName) {
    QFile localServerFile(QString("/tmp/%1").arg(localServerName));
    if (localServerFile.exists()) {
        qDebug("File \"%s\" already exists. It will be deleted now...", localServerFile.fileName().toLocal8Bit().data());
        if (!localServerFile.remove()) {
            qDebug("An error occurred while deleting file: \"%s\".", localServerFile.errorString().toLocal8Bit().data());
            return false;
        }
        qDebug("File deleted successfully.");
    }
    qDebug("Starting local server \"%s\"...", localServerName.toLocal8Bit().data());
    this -> localServer = new QLocalServer();
    connect(this -> localServer, SIGNAL(newConnection()), this, SLOT(on_localServer_newConnection()));
    if (!this -> localServer -> listen(localServerName)) {
        qDebug("An error occurred while starting local server: \"%s\".", this -> localServer -> errorString().toLocal8Bit().data());
        return false;
    }
    qDebug("Local server started successfully.");
    qDebug("Setting owner \"%s\" for file \"%s\"...", ownerUserName.toLocal8Bit().data(), localServerFile.fileName().toLocal8Bit().data());
    passwd* password_entry = getpwnam(ownerUserName.toLocal8Bit().data());
    if (password_entry == NULL) {
        qDebug("An error occurred while setting owner for file: (%i).", errno);
        if (errno == 0) {
            qDebug("User \"%s\" was not found.", ownerUserName.toLocal8Bit().data());
        }
        return false;
    }
    uid_t user_id  = password_entry -> pw_uid;
    gid_t group_id = -1;
    if (chown(localServerFile.fileName().toLocal8Bit().data(), user_id, group_id) == -1) {
        qDebug("An error occurred while setting owner for file: (%i).", errno);
        return false;
    }
    qDebug("Owner for file set successfully.");
    return true;
}

void GenericSensorServer::setLastError(QString identifier, QString message) {
    this -> sensorErrorsMap -> insert(identifier, message);
}

void GenericSensorServer::on_localServer_newConnection() {
    qDebug("Local client connected.");
    QLocalSocket* client = this -> localServer -> nextPendingConnection();
    this -> clientsList -> append(client);
    connect(client, SIGNAL(readyRead()), this, SLOT(on_client_readyRead()));
}

void GenericSensorServer::on_client_readyRead() {
    QLocalSocket* client = (QLocalSocket*)QObject::sender();
    QDataStream stream(client);
    QByteArray response = this -> parseRequest(&stream);
    stream.writeRawData(response.data(), response.length());
}

bool GenericSensorServer::getValue(QString identifier, double* value, QDateTime* timestamp) {
    *timestamp = QDateTime::currentDateTime();
    *value = this -> getCurrentValue(identifier);
    if (isnan(*value)) {
        return false;
    }
    this -> sensorErrorsMap -> insert(identifier, "No error");
    return true;
}

QByteArray GenericSensorServer::parseRequest(QDataStream* inputStream) {
    QByteArray outputData;
    QDataStream outputStream(&outputData, QIODevice::ReadWrite);
    quint8 commandType = 0;
    *inputStream >> commandType;
    switch (commandType) {
        case CommandTypeGetValue: {
            QString identifier;
            *inputStream >> identifier;
            qDebug("<- GetValue %s", identifier.toLocal8Bit().data());
            double value;
            QDateTime timestamp;
            if (this -> getValue(identifier, &value, &timestamp)) {
                outputStream << (quint8)CommandTypeOk << value << timestamp;
                qDebug("-> Ok %f %s", value, timestamp.toString("yyyy.MM.dd hh:mm:ss.zzz").toLocal8Bit().data());
            }
            else {
                outputStream << (quint8)CommandTypeError << this -> sensorErrorsMap -> value(identifier);
                qDebug("-> Error %s", this -> sensorErrorsMap -> value(identifier).toLocal8Bit().data());
            }
            return outputData;
        }
        default: {
            qDebug("<- Unknown command type %i", commandType);
            outputStream << (quint8)CommandTypeError << QString("Unknown command type");
            qDebug("-> Error %s", "Unknown command type");
            return outputData;
        }
    }
}
