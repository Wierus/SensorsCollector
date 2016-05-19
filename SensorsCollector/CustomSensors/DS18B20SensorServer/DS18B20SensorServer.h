#ifndef DS18B20SENSORSERVER_H
#define DS18B20SENSORSERVER_H

#include "GenericSensorServer.h"

#define SENSOR_FILE_PATH_TEMPLATE "/sys/bus/w1/devices/%1/w1_slave"
#define SENSOR_LINE0_CRC_INDEX    36
#define SENSOR_LINE1_VALUE_INDEX  29
#define SENSOR_CRC_CORRECT        "YES"
#define SENSOR_VALUE_MULTIPLIER   0.001

class DS18B20SensorServer : public GenericSensorServer {

public:

    DS18B20SensorServer();

protected:

    double getCurrentValue(QString identifier);

};

#endif // DS18B20SENSORSERVER_H
