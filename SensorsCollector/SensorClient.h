#ifndef SENSORCLIENT_H
#define SENSORCLIENT_H

#include <QDateTime>
#include <QLocalSocket>
#include <QTimer>
#include <QVariant>

#include <math.h>

#include "ApplicationLog.h"
#include "Consts.h"
#include "GenericSensorServer.h"

class SensorClient : public QObject {

Q_OBJECT

private:

    ApplicationLog* log;

    QLocalSocket* localClient;

    QTimer* refreshTimer;

    /**
     * @brief Номер датчика (поле "Sensors.Id" из БД).
     */
    unsigned int id;

    /**
     * @brief Тип датчика (поле "Sensors.Type" из БД).
     */
    QString type;

    /**
     * @brief Имя локального сервера, к которому будет выполняться подключение (поле "Types.ServerName" из БД).
     */
    QString serverName;

    /**
     * @brief Идентификатор датчика (поле "Sensors.Identifier" из БД).
     */
    QString identifier;

    /**
     * @brief Период обновления значений датчика (в секундах) (поле "Sensors.RefreshInterval" из БД).
     */
    unsigned int refreshInterval;

    bool connectToServer();

public:

    /**
     * @brief Инициализирует датчик с указанными параметрами.
     * @param log Лог для вывода сообщений.
     * @param id Номер датчика (поле "Sensors.Id" из БД).
     * @param type Тип датчика (поле "Sensors.Type" из БД).
     * @param serverName Имя локального сервера, к которому будет выполняться подключение (поле "Types.ServerName" из БД).
     * @param identifier Идентификатор датчика (поле "Sensors.Identifier" из БД).
     * @param refreshInterval Период обновления значений датчика (в секундах) (поле "Sensors.RefreshInterval" из БД).
     */
    SensorClient(ApplicationLog* log, unsigned int id, QString sensorType, QString serverName, QString identifier, unsigned int refreshInterval);

    /**
     * @brief Возвращает номер датчика (поле "Sensors.Id" из БД).
     * @return Номер датчика (поле "Sensors.Id" из БД).
     */
    unsigned int getId();

    /**
     * @brief Возвращает тип датчика (поле "Sensors.Type" из БД).
     * @return Тип датчика (поле "Sensors.Type" из БД).
     */
    QString getType();

    /**
     * @brief Возвращает имя локального сервера, к которому будет выполняться подключение (поле "Types.ServerName" из БД).
     * @return Имя локального сервера, к которому будет выполняться подключение (поле "Types.ServerName" из БД).
     */
    QString getServerName();

    /**
     * @brief Возвращает идентификатор датчика (поле "Sensors.Identifier" из БД).
     * @return Идентификатор датчика (поле "Sensors.Identifier" из БД).
     */
    QString getIdentifier();

    /**
     * @brief Возвращает период обновления значений датчика (в секундах) (поле "Sensors.RefreshInterval" из БД).
     * @return Период обновления значений датчика (в секундах) (поле "Sensors.RefreshInterval" из БД).
     */
    unsigned int getRefreshInterval();

    /**
     * @brief Запускает таймер для обновления значений датчика.
     */
    void startCollect();

    /**
     * @brief Отправляет запрос на получение значения датчика.
     */
    void sendGetRequest();

    void parseResponse(QDataStream* inputStream);

    QString toString();

private slots:

    void on_refreshTimer_timeout();

    void on_localClient_disconnected();
    void on_localClient_readyRead();

signals:

    /**
     * @brief Сигнал, вызываемый при успешном обновлении значения датчика (по таймеру).
     * @param value Значение датчика в момент измерения.
     * @param originDateTime Дата и время измерения значения датчика.
     */
    void valueRefreshSuccessed(double value, QDateTime originDateTime);

    /**
     * @brief Сигнал, вызываемый при возникновении ошибки во время получения значения датчика (по таймеру).
     * @param message Сообщение об ошибке.
     */
    void valueRefreshFailed(QString message);

};

#endif // SENSORCLIENT_H
