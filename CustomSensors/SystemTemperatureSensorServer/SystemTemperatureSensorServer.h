#ifndef SYSTEMTEMPERATURESENSORSERVER_H
#define SYSTEMTEMPERATURESENSORSERVER_H

#include "GenericSensorServer.h"

#define SENSOR_FILE_PATH        "/sys/class/thermal/thermal_zone0/temp"
#define SENSOR_VALUE_MULTIPLIER 0.001

class SystemTemperatureSensorServer : public GenericSensorServer {

public:

    SystemTemperatureSensorServer();

protected:

    double getCurrentValue(QString identifier);

};

#endif // SYSTEMTEMPERATURESENSORSERVER_H
