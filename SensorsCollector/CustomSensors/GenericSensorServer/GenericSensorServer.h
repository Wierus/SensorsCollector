#ifndef GENERICSENSORSERVER_H
#define GENERICSENSORSERVER_H

#include <QDataStream>
#include <QDateTime>
#include <QFile>
#include <QLocalServer>
#include <QLocalSocket>

#include <errno.h>
#include <math.h>
#include <pwd.h>
#include <unistd.h>

/**
 * @brief Представляет множество датчиков одного типа.
 * Запускает один локальный сервер, который может принимать подключения от множества локальных клиентов.
 */
class GenericSensorServer : public QObject {

Q_OBJECT

private:

    /**
     * @brief Локальный сервер.
     */
    QLocalServer* localServer;

    /**
     * @brief Список подключенных локальных клиентов.
     */
    QList<QLocalSocket*>* clientsList;

    /**
     * @brief Список сообщений об ошибках для каждого из датчиков.
     */
    QMap<QString, QString>* sensorErrorsMap;

    /**
     * @brief Запускает процесс измерения значения определенного датчика и синхронно возвращает это значение.
     * @param identifier Идентификатор датчика.
     * @param value Значение датчика в момент измерения.
     * @param timestamp Дата и время измерения значения датчика.
     * @return true, если значение датчика успешно получено; false, если при получении значения датчика произошла ошибка.
     */
    bool getValue(QString identifier, double* value, QDateTime* timestamp);

    QByteArray parseRequest(QDataStream* inputStream);

protected:

    /**
     * @brief Вычисляет и возвращает текущее значение датчика. Вычисление значения датчика выполняется синхронно.
     * @param identifier Идентификатор датчика.
     * @return Текущее значение датчика. NAN, если при получении значения датчика произошла ошибка.
     */
    virtual double getCurrentValue(QString identifier) = 0;

    /**
     * @brief Устанавливает текст сообщения об ошибке при получении значения датчика.
     * Этот метод должен быть вызван в наследуемых классах перед возвратом значения NAN из метода getCurrentValue().
     * @param identifier Идентификатор датчика.
     * @param message Текст сообщения.
     */
    void setLastError(QString identifier, QString message);

public:

    GenericSensorServer();

    bool initializeLocalServer(QString localServerName, QString ownerUserName);

    enum CommandType {

        CommandTypeGetValue = 0x01,
        CommandTypeOk       = 0x02,
        CommandTypeError    = 0x03,

    };

private slots:

    void on_localServer_newConnection();

    void on_client_readyRead();

};

#endif // GENERICSENSORSERVER_H
