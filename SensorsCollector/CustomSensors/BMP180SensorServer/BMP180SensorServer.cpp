#include "BMP180SensorServer.h"

BMP180SensorServer::BMP180SensorServer() {
    if (wiringPiSetup() == 0) {
        qDebug("WiringPi initialized successfully");
    }
    else {
        qDebug("Error occurred while initializing WiringPi");
    }
    this -> fd = wiringPiI2CSetup(BMP180_I2C_ADDRESS);
    if (this -> fd == -1) {
        qDebug("An error occurred while setting up I2C bus at address \"0x%X\": (%d).", BMP180_I2C_ADDRESS, errno);
    }
}

bool BMP180SensorServer::initializeSensor(unsigned char mode) {
    if (this -> readReg8(BMP180_ID_REGISTER_ADDRESS) != BMP180_ID_REGISTER_VALUE) {
        qDebug("Could not find a valid BMP180 sensor at address \"0x%X\".", BMP180_I2C_ADDRESS);
        return false;
    }
    if (mode > BMP180_OSS_ULTRA_HIGH_RESOLUTION_MODE) {
        mode = BMP180_OSS_ULTRA_HIGH_RESOLUTION_MODE;
    }
    this -> oversampling = mode;
    this -> ac1 = readReg16(BMP180_CAL_AC1);
    this -> ac2 = readReg16(BMP180_CAL_AC2);
    this -> ac3 = readReg16(BMP180_CAL_AC3);
    this -> ac4 = readReg16(BMP180_CAL_AC4);
    this -> ac5 = readReg16(BMP180_CAL_AC5);
    this -> ac6 = readReg16(BMP180_CAL_AC6);
    this -> b1 = readReg16(BMP180_CAL_B1);
    this -> b2 = readReg16(BMP180_CAL_B2);
    this -> mb = readReg16(BMP180_CAL_MB);
    this -> mc = readReg16(BMP180_CAL_MC);
    this -> md = readReg16(BMP180_CAL_MD);
    return true;
}

long BMP180SensorServer::computeB5(long UT) {
    long X1 = ((UT - (long)ac6) * (long)ac5) >> 15;
    long X2 = ((long)mc << 11) / (X1 + (long)md);
    return X1 + X2;
}

long BMP180SensorServer::readRawTemperature() {
    this -> writeReg8(BMP180_CONTROL_REGISTER_ADDRESS, BMP180_READ_TEMPERATURE_COMMAND);
    delay(BMP180_TEMPERATURE_WAIT);
    long raw = this -> readReg16(BMP180_DATA_REGISTER_ADDRESS);
    return raw;
}

long BMP180SensorServer::readRawPressure() {
    this -> writeReg8(BMP180_CONTROL_REGISTER_ADDRESS, BMP180_READ_PRESSURE_COMMAND + (this -> oversampling << 6));
    switch (this -> oversampling) {
        case BMP180_OSS_ULTRA_LOW_POWER_MODE: {
            delay(BMP180_OSS_ULTRA_LOW_POWER_WAIT);
            break;
        }
        case BMP180_OSS_STANDARD_MODE: {
            delay(BMP180_OSS_STANDARD_WAIT);
            break;
        }
        case BMP180_OSS_HIGH_RESOLUTION_MODE: {
            delay(BMP180_OSS_HIGH_RESOLUTION_WAIT);
            break;
        }
        case BMP180_OSS_ULTRA_HIGH_RESOLUTION_MODE: {
            delay(BMP180_OSS_ULTRA_HIGH_RESOLUTION_WAIT);
            break;
        }
    }
    long msb  = this -> readReg8(BMP180_DATA_REGISTER_ADDRESS);
    long lsb  = this -> readReg8(BMP180_DATA_REGISTER_ADDRESS + 1);
    long xlsb = this -> readReg8(BMP180_DATA_REGISTER_ADDRESS + 2);
    long raw = ((msb << 16) | (lsb << 8) | xlsb) >> (8 - this -> oversampling);
    return raw;
}

double BMP180SensorServer::readTemperature() {
    long UT = readRawTemperature();
    long B5 = computeB5(UT);
    long t = (B5 + 8) >> 4;
    return t / 10.0;
}

double BMP180SensorServer::readPressure() {
    long UT = readRawTemperature();
    long UP = readRawPressure();
    long B5 = computeB5(UT);
    long B6 = B5 - 4000;
    long X1 = ((long)b2 * ((B6 * B6) >> 12)) >> 11;
    long X2 = ((long)ac2 * B6) >> 11;
    long X3 = X1 + X2;
    long B3 = ((((long)ac1 * 4 + X3) << oversampling) + 2) / 4;
    X1 = ((long)ac3 * B6) >> 13;
    X2 = ((long)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    unsigned long B4 = ((unsigned long)ac4 * (unsigned long)(X3 + 32768)) >> 15;
    unsigned long B7 = ((unsigned long)UP - B3) * (unsigned long)(50000 >> oversampling);
    long p;
    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    }
    else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + 3791) >> 4);
    return p;
}

double BMP180SensorServer::readAltitude(double sealevelPressure) {
    double pressure = readPressure();
    double altitude = 44330.0 * (1.0 - qPow(pressure / sealevelPressure, 1.0 / 5.255));
    return altitude;
}

double BMP180SensorServer::readSealevelPressure(double altitude) {
    double pressure = readPressure();
    double sealevelPressure = pressure / qPow(1.0 - altitude / 44330.0, 5.255);
    return sealevelPressure;
}

unsigned char BMP180SensorServer::readReg8(unsigned char address) {
    unsigned char ret = wiringPiI2CReadReg8(this -> fd, address);
    return ret;
}

unsigned short BMP180SensorServer::readReg16(unsigned char address) {
    unsigned short data = wiringPiI2CReadReg16(this -> fd, address);
    data = ((data << 8) & 0xFF00) | ((data >> 8) & 0x00FF);
    return data;
}

void BMP180SensorServer::writeReg8(unsigned char address, unsigned char data) {
    wiringPiI2CWriteReg8(this -> fd, address, data);
}

double BMP180SensorServer::getCurrentValue(QString identifier) {
    if (!this -> initializeSensor(BMP180_OSS_ULTRA_HIGH_RESOLUTION_MODE)) {
        QString lastError = QString("An error occured while initializing BMP180 sensor.");
        this -> setLastError(identifier, lastError);
        qDebug(lastError.toLocal8Bit().data());
        return NAN;
    }
    double pressure = this -> readPressure();
    return pressure;
}
