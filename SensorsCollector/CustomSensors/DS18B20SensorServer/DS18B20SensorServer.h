#ifndef DS18B20SENSORSERVER_H
#define DS18B20SENSORSERVER_H

#include "GenericSensorServer.h"

#define DS18B20_FILE_PATH_TEMPLATE "/sys/bus/w1/devices/%1/w1_slave"
#define DS18B20_LINE0_CRC_INDEX    36
#define DS18B20_LINE1_VALUE_INDEX  29
#define DS18B20_CRC_CORRECT        "YES"
#define DS18B20_VALUE_MULTIPLIER   0.001

class DS18B20SensorServer : public GenericSensorServer {

public:

    DS18B20SensorServer();

protected:

    double getCurrentValue(QString identifier);

};

#endif // DS18B20SENSORSERVER_H
