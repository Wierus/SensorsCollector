#include "SensorsCollector.h"

SensorsCollector::SensorsCollector(ApplicationLog* log) {
    this -> log = log;
}

bool SensorsCollector::initialize() {
    if (!this -> initializeDatabaseConnection()) {
        return false;
    }
    if (!this -> initializeSensorsFromDatabase()) {
        return false;
    }
    return true;
}

bool SensorsCollector::initializeDatabaseConnection() {
    this -> log -> write(QString("Checking the availability of SQL driver \"%1\"...").arg(DATABASE_DRIVER_TYPE));
    if (!QSqlDatabase::isDriverAvailable(DATABASE_DRIVER_TYPE)) {
        this -> log -> write(QString("SQL driver \"%1\" is not available. Connecting to the database server can not be performed.").arg(DATABASE_DRIVER_TYPE));
        return false;
    }
    this -> log -> write(QString("SQL driver \"%1\" is available. It will be used for connecting to the database server.").arg(DATABASE_DRIVER_TYPE));
    this -> database = new QSqlDatabase(QSqlDatabase::addDatabase(DATABASE_DRIVER_TYPE));
    this -> database -> setHostName(DATABASE_HOST_NAME);
    this -> database -> setDatabaseName(DATABASE_NAME);
    this -> database -> setPort(DATABASE_PORT);
    this -> database -> setUserName(DATABASE_USER_NAME);
    this -> database -> setPassword(DATABASE_PASSWORD);
    this -> log -> write(QString("Connecting to the database server \"%1:%2\"...").arg(DATABASE_HOST_NAME).arg(DATABASE_PORT));
    if (!this -> database -> open()) {
        QSqlError error = this -> database -> lastError();
        this -> log -> write(QString("An error occurred while connecting to the database server: (%1) %2").arg(error.number()).arg(error.text()));
        return false;
    }
    this -> log -> write(QString("Connection to the database server established successfully."));
    return true;
}

bool SensorsCollector::initializeSensorsFromDatabase() {
    this -> sensorsList = new QList<SensorClient*>();
    this -> log -> write(QString("Retrieving the list of active sensors (with flag \"IsUse\") from the database..."));
    QSqlQuery query;
    if (!query.exec("SELECT `Sensors`.`Id`, `Sensors`.`Type`, `Types`.`ServerName`, `Sensors`.`Identifier`, `Sensors`.`RefreshInterval` FROM `Sensors` INNER JOIN `Types` USING (`Type`) WHERE (`IsUse` = TRUE)")) {
        this -> log -> write(QString("An error occurred while retrieving the list of active sensors (with flag \"IsUse\") from the database: (%1) %2.").arg(query.lastError().number()).arg(query.lastError().text()));
        return false;
    }
    int successSensorsCount = 0;
    int failSensorsCount    = 0;
    int resultIdFieldNo              = query.record().indexOf("Id");
    int resultTypeFieldNo            = query.record().indexOf("Type");
    int resultServerNameFieldNo      = query.record().indexOf("ServerName");
    int resultIdentifierFieldNo      = query.record().indexOf("Identifier");
    int resultRefreshIntervalFieldNo = query.record().indexOf("RefreshInterval");
    while (query.next()) {
        unsigned int resultId              = query.value(resultIdFieldNo).toUInt();
        QString      resultType            = query.value(resultTypeFieldNo).toString();
        QString      resultServerName      = query.value(resultServerNameFieldNo).toString();
        QString      resultIdentifier      = query.value(resultIdentifierFieldNo).toString();
        unsigned int resultRefreshInterval = query.value(resultRefreshIntervalFieldNo).toUInt();
        if (resultType.isEmpty()) {
            failSensorsCount++;
            this -> log -> write(QString("An error occured while initializing sensor {Id = \"%1\"}: sensor type is not specified, sensor can not be initialized.").arg(resultId));
            continue;
        }
        SensorClient* sensor = new SensorClient(this -> log, resultId, resultType, resultServerName, resultIdentifier, resultRefreshInterval);
        this -> sensorsList -> append(sensor);
        successSensorsCount++;
        this -> log -> write(QString("Sensor %1 initialized successfully.").arg(sensor -> toString()));
    }
    unsigned int totalSensorsCount = successSensorsCount + failSensorsCount;
    this -> log -> write(QString("Total sensors processed from the database: %1.").arg(totalSensorsCount));
    this -> log -> write(QString("Sensors initialized successfully: %1.").arg(successSensorsCount));
    this -> log -> write(QString("Sensors initialized with error: %1.").arg(failSensorsCount));
    return true;
}

bool SensorsCollector::startCollect() {
    if (this -> sensorsList == NULL) {
        this -> log -> write(QString("List of sensors has not been initialized."));
        return false;
    }
    for (QList<SensorClient*>::iterator i = this -> sensorsList -> begin(); i != this -> sensorsList -> end(); i++) {
        connect(*i, SIGNAL(valueRefreshSuccessed(double, QDateTime)), this, SLOT(on_sensor_refreshSuccessed(double, QDateTime)));
        connect(*i, SIGNAL(valueRefreshFailed(QString)),              this, SLOT(on_sensor_refreshFailed(QString)));
        (*i) -> startCollect();
    }
    return true;
}

void SensorsCollector::on_sensor_refreshSuccessed(double value, QDateTime originDateTime) {
    SensorClient* sensor = (SensorClient*)QObject::sender();
    QSqlQuery query;
    if (!query.prepare("INSERT INTO `Values` (`DateTime`, `SensorId`, `Value`) VALUES (:DateTime, :SensorId, :Value)")) {
        this -> log -> write(QString("An error occurred while preparing query for inserting sensor value %1 into table: (%2) %3.").arg(sensor -> toString()).arg(query.lastError().number()).arg(query.lastError().text()));
        return;
    }
    query.bindValue(":DateTime", originDateTime);
    query.bindValue(":SensorId", sensor -> getId());
    query.bindValue(":Value", value);
    if (!query.exec()) {
        this -> log -> write(QString("An error occurred while executing query for inserting sensor value %1 into table: (%2) %3.").arg(sensor -> toString()).arg(query.lastError().number()).arg(query.lastError().text()));
        return;
    }
}

void SensorsCollector::on_sensor_refreshFailed(QString message) {
    SensorClient* sensor = (SensorClient*)QObject::sender();
    this -> log -> write(QString("An error occurred while receiving sensor value %1: %2").arg(sensor -> toString()).arg(message));
}
