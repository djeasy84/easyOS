/*
 * MIT License
 *
 * This file is part of easyOS.
 * You can find the software at https://github.com/djeasy84/easyOS
 *
 * Copyright (c) 2018 - Lorenzo Baglioni (djeasy84) <lbaglioni84@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef MPU6050_H
#define MPU6050_H

#define I2C_ACCGYRO_ID 0x68

class AccelerometerGyroscope
{
    public:
        bool setup(uint16_t accRange = 16, uint16_t gyroRange = 2000, uint8_t lowPassFilterRange = 0);

        bool read(float *accX, float *accY, float *accZ, float *gyroX, float *gyroY, float *gyroZ);

    private:
        bool setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t data);
        bool setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len);
        bool setRegistryBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, bool value);

        bool getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
        bool getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len);

        bool setupAccelerometerRange(uint16_t range = 16);
        bool setupGyroscopeRange(uint16_t range = 2000);
        bool setupLowPassFilter(uint8_t range = 0);

        float mapRawToFloat(uint8_t RawH, uint8_t RawL, float range);

        float accRange, gyroRange;
};

AccelerometerGyroscope AG;

/****************************************************************************************/

bool AccelerometerGyroscope::setup(uint16_t accRange, uint16_t gyroRange, uint8_t lowPassFilterValue)
{
    if (!setupAccelerometerRange(accRange))
        return false;
    if (!setupGyroscopeRange(gyroRange))
        return false;
    if (!setupLowPassFilter(lowPassFilterValue))
      return false;
    return setRegistryBit(I2C_ACCGYRO_ID, 0x6B, 6, false);
}

bool AccelerometerGyroscope::read(float *accX, float *accY, float *accZ, float *gyroX, float *gyroY, float *gyroZ)
{
    uint8_t buffer[14] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if (!getRegistry(I2C_ACCGYRO_ID, 0x3B, buffer, 14))
    {
        //*accX = 0.0;
        //*accY = 0.0;
        //*accZ = 0.0;
        //*gyroX = 0.0;
        //*gyroY = 0.0;
        //*gyroZ = 0.0;
        return false;
    }
    *accX = mapRawToFloat(buffer[0], buffer[1], accRange);
    *accY = mapRawToFloat(buffer[2], buffer[3], accRange);
    *accZ = mapRawToFloat(buffer[4], buffer[5], accRange);
    *gyroX = mapRawToFloat(buffer[8], buffer[9], gyroRange);
    *gyroY = mapRawToFloat(buffer[10], buffer[11], gyroRange);
    *gyroZ = mapRawToFloat(buffer[12], buffer[13], gyroRange);
    return true;
}

bool AccelerometerGyroscope::setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return setRegistry(devAddr, regAddr, &data, 1);
}

bool AccelerometerGyroscope::setRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, false))
        return false;
    if (!I2C.sendData(regAddr))
        return false;
    if (!I2C.sendData(*data))
        return false;
    if (!I2C.sendStop())
        return false;
    return true;
}

bool AccelerometerGyroscope::setRegistryBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitPos, bool value)
{
    uint8_t regValue = 0;
    if (!getRegistry(devAddr, regAddr, &regValue))
        return false;
    regValue = (value) ? (regValue | (1<<bitPos)) : (regValue & ~(1<<bitPos));
    return setRegistry(devAddr, regAddr, regValue);
}

bool AccelerometerGyroscope::getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
    return getRegistry(devAddr, regAddr, data, 1);
}

bool AccelerometerGyroscope::getRegistry(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint8_t len)
{
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, false))
        return false;
    if (!I2C.sendData(regAddr))
        return false;
    if (!I2C.sendStart())
        return false;
    if (!I2C.sendDeviceAddressWithReadWrite(devAddr, true))
        return false;
    for (uint8_t idx=0; idx<len; idx++)
    {
        if (!I2C.readData(&data[idx], (idx!=(len-1))?false:true))
            return false;
    }
    if (!I2C.sendStop())
        return false;
    return true;
}

bool AccelerometerGyroscope::setupAccelerometerRange(uint16_t range)
{
    switch(range)
    {
        case 2:
        {
            accRange = 2;
            return setRegistry(I2C_ACCGYRO_ID, 0x1C, (0 << 3));
        }
        break;
        case 4:
        {
            accRange = 4;
            return setRegistry(I2C_ACCGYRO_ID, 0x1C, (1 << 3));
        }
        break;
        case 8:
        {
            accRange = 8;
            return setRegistry(I2C_ACCGYRO_ID, 0x1C, (2 << 3));
        }
        break;
        case 16:
        default:
        {
            accRange = 16;
            return setRegistry(I2C_ACCGYRO_ID, 0x1C, (3 << 3));
        }
        break;
    }
    return false;
}

bool AccelerometerGyroscope::setupGyroscopeRange(uint16_t range)
{
    switch(range)
    {
        case 250:
        {
            gyroRange = 250;
            return setRegistry(I2C_ACCGYRO_ID, 0x1B, (0 << 3));
        }
        break;
        case 500:
        {
            gyroRange = 500;
            return setRegistry(I2C_ACCGYRO_ID, 0x1B, (1 << 3));
        }
        break;
        case 1000:
        {
            gyroRange = 1000;
            return setRegistry(I2C_ACCGYRO_ID, 0x1B, (2 << 3));
        }
        break;
        case 2000:
        default:
        {
            gyroRange = 2000;
            return setRegistry(I2C_ACCGYRO_ID, 0x1B, (3 << 3));
        }
        break;
    }
    return false;
}

bool AccelerometerGyroscope::setupLowPassFilter(uint8_t range)
{
    if (range > 6)
        return false;
    return setRegistry(I2C_ACCGYRO_ID, 0x1A, range);
}

float AccelerometerGyroscope::mapRawToFloat(uint8_t RawH, uint8_t RawL, float range)
{
    int16_t value = (RawH << 8) | RawL;
    return value * ((range * 2) / 65535);
}

#endif
