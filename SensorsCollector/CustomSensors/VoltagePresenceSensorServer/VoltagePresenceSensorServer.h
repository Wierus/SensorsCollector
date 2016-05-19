#ifndef VOLTAGEPRESENCESENSORSERVER_H
#define VOLTAGEPRESENCESENSORSERVER_H

#include "GenericSensorServer.h"

#include <wiringPi.h>

class VoltagePresenceSensorServer : public GenericSensorServer {

public:

    VoltagePresenceSensorServer();

protected:

    double getCurrentValue(QString identifier);

};

#endif // VOLTAGEPRESENCESENSORSERVER_H
