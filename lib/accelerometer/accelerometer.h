#ifndef INC_2026_IREC_PAYLOAD_ACCELEROMETER_H
#define INC_2026_IREC_PAYLOAD_ACCELEROMETER_H

#include <Adafruit_LSM6DSL.h>

class accelerometer
{
public:
    accelerometer();

    /**
     * Attempts to find and initialize the accelerometer at I2C address 0x6A with the following pinouts:
     * - 5V -> VIN
     * - GND -> GND
     * - SCL -> SCL
     * - SDA -> SDA
     * 
     * @return True if successfully found and initialized the sensor; false otherwise
     */
    bool init();

    /**
     * Attempts to read data from the accelerometer
     * @param x Acceleration in x, in g's
     * @param y Acceleration in y (axis of the rocket), in g's
     * @param z Acceleration in z, in g's
     * @return True if successfully read; false otherwise
     */
    bool get_acceleration(float &x, float &y, float &z);
private:
    Adafruit_LSM6DSL lsm6dsl;
};



#endif