#ifndef INC_2026_IREC_PAYLOAD_ACCELEROMETER_H
#define INC_2026_IREC_PAYLOAD_ACCELEROMETER_H

#include <../../.pio/libdeps/uno/MPU6050/src/MPU6050.h> 

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
} accelerometer_data;

class accelerometer
{
public:
    accelerometer();

    /**
     * Attempts to find and initialize the accelerometer at I2C address 0x68 with the following pinouts:
     * - 5V -> VIN
     * - GND -> GND
     * - SCL -> SCL
     * - SDA -> SDA
     * 
     * @return True if successfully found and initialized the sensor; false otherwise
     */
    bool init();

    /**
     * Attempts to read data from the accelerometer and store it in the accelerometer_data struct
     * @param data accelerometer_data struct where the read data will be stored
     * @return True if successfully read; false otherwise
     */
    void read_data(accelerometer_data &data);

    /**
     * Returns the current range of the accelerometer
     * @return The current range of the accelerometer
     */
    uint8_t get_accel_range();

    /**
     * Sets the new range for the accelerometer
     * @param range New range for the accelerometer
     */
    void set_accel_range(uint8_t range);
private:
    MPU6050 mpu;
};



#endif