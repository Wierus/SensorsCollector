#ifndef SENSORSCOLLECTOR_H
#define SENSORSCOLLECTOR_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "ApplicationLog.h"
#include "SensorClient.h"

class SensorsCollector : public QObject {

Q_OBJECT

private:

    ApplicationLog* log;

    /**
     * @brief Подключение к базе данных.
     */
    QSqlDatabase* database;

    /**
     * @brief Список датчиков.
     */
    QList<SensorClient*>* sensorsList;

    bool initializeDatabaseConnection();

    bool initializeSensorsFromDatabase();

public:

    SensorsCollector(ApplicationLog* log);

    bool initialize();

    bool startCollect();

private slots:

    /**
     * @brief Обработчик сигнала при обновлении значения датчика.
     * @param value Значения датчика в момент измерения.
     * @param originDateTime Дата и время измерения значения датчика.
     */
    void on_sensor_refreshSuccessed(double value, QDateTime originDateTime);

    /**
     * @brief Обработчик сигнала при возникновении ошибки во время получения значения датчика.
     * @param message Сообщение об ошибке.
     */
    void on_sensor_refreshFailed(QString message);

};

#endif // SENSORSCOLLECTOR_H
