#ifndef VOLTAGEPRESENCESENSORSERVER_H
#define VOLTAGEPRESENCESENSORSERVER_H

#include "GenericSensorServer.h"

#include <wiringPi.h>

class VoltagePresenceSensorServer : public GenericSensorServer {

public:

    VoltagePresenceSensorServer();

protected:

    /**
     * @brief Вычисляет и возвращает текущее значение датчика. Вычисление значения датчика выполняется синхронно.
     * @param identifier Идентификатор датчика - номер вывода, к которому подключен датчик (согласно нумерации в библиотеке WiringPi).
     * @return Текущее значение датчика. NAN, если при получении значения датчика произошла ошибка.
     */
    double getCurrentValue(QString identifier);

};

#endif // VOLTAGEPRESENCESENSORSERVER_H
