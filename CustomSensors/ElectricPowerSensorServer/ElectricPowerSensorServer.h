#ifndef ELECTRICPOWERSENSORSERVER_H
#define ELECTRICPOWERSENSORSERVER_H

#include "GenericSensorServer.h"

#include <wiringPi.h>

/** Максимальное количество обработчиков импульсов.
  */
#define SENSOR_MAX_COUNT 2

/** Минимально допустимый интервал между импульсами (мс).
 * В течение этого интервала последующие импульсы будут проигнорированы.
 */
#define SENSOR_IMPULSES_SPAN_IGNORE_TIME 100

/** Минимальный интервал между импульсами, требующий внимания (мс).
 * Должен быть больше SENSOR_IMPULSES_SPAN_IGNORE_TIME.
 * В течение этого интервала последующие имульсы будут штатно обработаны и данные о них будут дополнительно занесены в журнал.
 */
#define SENSOR_IMPULSES_SPAN_WARNING_TIME 150

class ElectricPowerSensorServer : public GenericSensorServer {

public:

    ElectricPowerSensorServer();

protected:

    double getCurrentValue(QString identifier);

private:

    struct SensorData {

        qint64 lastImpulseTime;

        int impulsesCount;

        qint64 impulsesSpanMs;

    };

    static QMap<int, SensorData>* sensorsDataMap;

    static int interruptHandlersPinsArray[SENSOR_MAX_COUNT];

    static void interruptHandler0();

    static void interruptHandler1();

    static void interruptHandler(const int pin);

};

#endif // ELECTRICPOWERSENSORSERVER_H
