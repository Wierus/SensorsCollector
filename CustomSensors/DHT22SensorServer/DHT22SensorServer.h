#ifndef DHT22SENSORSERVER_H
#define DHT22SENSORSERVER_H

#include "GenericSensorServer.h"

#include <errno.h>
#include <math.h>

#include <wiringPi.h>

/** Количество байт данных, читаемых из датчика.
 * Последовательность байт:
 * Byte 0: Humidity high
 * Byte 1: Humidity low
 * Byte 2: Temperature high
 * Byte 3: Temperature low
 * Byte 4: Parity bits
 */
#define SENSOR_DATA_LENGTH 5

/** Host the start signal down time (ms).
 */
#define SENSOR_T_be  1

/** Bus master has released time (us).
 */
#define SENSOR_T_go 30

/** Signal "0" high time (us).
 */
#define SENSOR_T_H0 26

/** Signal "1" high time (us).
 */
#define SENSOR_T_H1 70

/** Максимальное время ожидания изменения уровня сигнала на шине (мкс).
 */
#define SENSOR_TIMEOUT 85

/** Битовая маска знака температуры (бит "1" - знак).
 */
#define SENSOR_TEMPERATURE_SIGN_MASK 0b1000000000000000

/** Интервал повторного опроса датчика в случае неудачной попытки (мс).
 */
#define SENSOR_FAILED_REPEAT_INTERVAL 2000

/** Максимальное количество допустимых неудачных попыток для получения значения датчика.
 */
#define SENSOR_MAX_FAILED_ATTEMPTS 5

class DHT22SensorServer : public GenericSensorServer {

Q_OBJECT

public:

    DHT22SensorServer();

    /** Коды ошибок при выполнении операций.
     */
    enum ErrorCodes {

        /** Операция выполнена успешно.
         */
        OperationOK,

        /** Сигнал присутствия (Response signal) не обнаружен, устройство отсутствует на шине.
         */
        ResponseSignalNotDetected,

        /** Ошибка при чтении данных из датчика, датчик не ответил вовремя.
         */
        TimeoutReadError,

        /** Ошибка при чтении данных из датчика, один или несколько битов четности не совпадают.
         */
        ParityReadError,

    };

protected:

    /**
     * @brief Вычисляет и возвращает текущее значение датчика. Вычисление значения датчика выполняется синхронно.
     * @param identifier Идентификатор датчика - номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @return Текущее значение датчика. NAN, если при получении значения датчика произошла ошибка.
     */
    double getCurrentValue(QString identifier);

private:

    /**
     * @brief Читает данные из датчика и сохраняет полученные значения в переменных: humidity (влажность) и temperature (температура).
     * @param pin Номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @param humidity Указатель на переменную, в которую будет сохранено значение влажности.
     * @param temperature Указатель на переменную, в которую будет сохранено значение температуры.
     * @return Код ошибки или успешного завершения операции.
     */
    ErrorCodes readDHT22(const int pin, double* humidity, double* temperature);

    /**
     * @brief Отправляет сигнал датчику для его включения и ждет ответ.
     * @param pin Номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @return true, если ответ успешно получен. false, если датчик не ответил вовремя.
     */
    bool sendStartSignal(const int pin);

    /**
     * @brief Ожидает на шине сначала низкий уровень, затем высокий.
     * @param pin Номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @return true, если на шине был прочитан сначала низкий уровень, затем высокий уровень. false, если какой-либо уровень не был прочитан вовремя.
     */
    bool waitLowHigh(const int pin);

    /**
     * @brief Читает из датчика 5 байт данных.
     * @param pin Номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @param data Массив для записи данных.
     * @return true, если данные прочитаны успешно (без проверки контрольной суммы). false, если при чтении данных датчик не ответил вовремя.
     */
    bool readData(const int pin, unsigned char data[]);

    /**
     * @brief Читает из датчика 1 байт данных.
     * @param pin Номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @param byte Указатель на переменную для записи байта.
     * @return true, если байт прочитан успешно. false, если при чтении данных датчик не ответил вовремя.
     */
    bool readByte(const int pin, unsigned char* byte);

};

#endif // DHT22SENSORSERVER_H
