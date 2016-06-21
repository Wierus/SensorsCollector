#ifndef BMP180SENSORSERVER_H
#define BMP180SENSORSERVER_H

#include "GenericSensorServer.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <QtMath>

/** Identifier of sensor at I2C-bus (can be defined with command "gpio i2cdetect").
 */
#define BMP180_I2C_ADDRESS 0x77

/** Pressure oversampling mode "Ultra low power" (internal number of samples: 1).
 */
#define BMP180_OSS_ULTRA_LOW_POWER_MODE       0

/** Pressure oversampling mode "Standard" (internal number of samples: 2).
 */
#define BMP180_OSS_STANDARD_MODE              1

/** Pressure oversampling mode "High resolution" (internal number of samples: 4).
 */
#define BMP180_OSS_HIGH_RESOLUTION_MODE       2

/** Pressure oversampling mode "Ultra high resolution" (internal number of samples: 8).
 */
#define BMP180_OSS_ULTRA_HIGH_RESOLUTION_MODE 3

/** Waiting time for reading pressure value in mode "Ultra low power" (ms).
 */
#define BMP180_OSS_ULTRA_LOW_POWER_WAIT        5

/** Waiting time for reading pressure value in mode "Standard" (ms).
 */
#define BMP180_OSS_STANDARD_WAIT               8

/** Waiting time for reading pressure value in mode "High resolution" (ms).
 */
#define BMP180_OSS_HIGH_RESOLUTION_WAIT       14

/** Waiting time for reading pressure value in mode "Ultra high resolution" (ms).
 */
#define BMP180_OSS_ULTRA_HIGH_RESOLUTION_WAIT 26

/** Waiting time for reading temperature value (ms).
 */
#define BMP180_TEMPERATURE_WAIT 5

#define BMP180_CAL_AC1 0xAA
#define BMP180_CAL_AC2 0xAC
#define BMP180_CAL_AC3 0xAE
#define BMP180_CAL_AC4 0xB0
#define BMP180_CAL_AC5 0xB2
#define BMP180_CAL_AC6 0xB4
#define BMP180_CAL_B1  0xB6
#define BMP180_CAL_B2  0xB8
#define BMP180_CAL_MB  0xBA
#define BMP180_CAL_MC  0xBC
#define BMP180_CAL_MD  0xBE

#define BMP180_ID_REGISTER_ADDRESS 0xD0
#define BMP180_ID_REGISTER_VALUE   0x55

#define BMP180_CONTROL_REGISTER_ADDRESS 0xF4
#define BMP180_DATA_REGISTER_ADDRESS    0xF6

#define BMP180_READ_TEMPERATURE_COMMAND 0x2E
#define BMP180_READ_PRESSURE_COMMAND    0x34

class BMP180SensorServer : public GenericSensorServer {

public:

    BMP180SensorServer();

    bool initializeSensor(unsigned char mode);

    double readTemperature();

    double readPressure();

    double readAltitude(double sealevelPressure = 101325.0);

    double readSealevelPressure(double altitude = 0.0);

protected:

    /**
     * @brief Вычисляет и возвращает текущее значение датчика. Вычисление значения датчика выполняется синхронно.
     * @param identifier Не используется при вычислении. Может иметь любое значение.
     * @return Текущее значение датчика. NAN, если при получении значения датчика произошла ошибка.
     */
    double getCurrentValue(QString identifier);

private:

    /**
     * @brief Standard Linux file handle for I2C device.
     */
    int fd;

    /**
     * @brief Pressure oversampling mode (controls the oversampling ratio of the pressure measurement).
     */
    unsigned char oversampling;

    short ac1, ac2, ac3, b1, b2, mb, mc, md;
    unsigned short ac4, ac5, ac6;

    long computeB5(long UT);

    /**
     * @brief Reads the raw measured temperature value of this sensor.
     * @return Raw measured temperature.
     */
    long readRawTemperature();

    /**
     * @brief Reads the raw measured pressure value of this sensor.
     * @return Raw measured pressure.
     */
    long readRawPressure();

    /**
     * @brief Read 8-bit data value from the device register indicated.
     * @param address Address of register.
     * @return 8-bit data value.
     */
    unsigned char readReg8(unsigned char address);

    /**
     * @brief Read 16-bit data value from the device register indicated.
     * @param address Address of register.
     * @return 16-bit data value.
     */
    unsigned short readReg16(unsigned char address);

    /**
     * @brief Writes 8-bit data value into the device register indicated.
     * @param address Address of register.
     * @param data 8-bit data value.
     */
    void writeReg8(unsigned char address, unsigned char data);

};

#endif // BMP180SENSORSERVER_H
