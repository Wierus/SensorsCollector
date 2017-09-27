#ifndef SYSTEMTEMPERATURESENSORSERVER_H
#define SYSTEMTEMPERATURESENSORSERVER_H

#include "GenericSensorServer.h"

#define SYSTEM_TEMPERATURE_FILE_PATH        "/sys/class/thermal/thermal_zone0/temp"
#define SYSTEM_TEMPERATURE_VALUE_MULTIPLIER 0.001

class SystemTemperatureSensorServer : public GenericSensorServer {

public:

    SystemTemperatureSensorServer();

protected:

    /**
     * @brief Вычисляет и возвращает текущее значение датчика. Вычисление значения датчика выполняется синхронно.
     * @param identifier Не используется при вычислении. Может иметь любое значение.
     * @return Текущее значение датчика. NAN, если при получении значения датчика произошла ошибка.
     */
    double getCurrentValue(QString identifier);

};

#endif // SYSTEMTEMPERATURESENSORSERVER_H
