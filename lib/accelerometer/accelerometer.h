#ifndef INC_2026_IREC_PAYLOAD_ACCELEROMETER_H
#define INC_2026_IREC_PAYLOAD_ACCELEROMETER_H

#include <Adafruit_MPU6050.h> 

typedef struct
{
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
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
    mpu6050_accel_range_t get_accel_range();

    /**
     * Sets the new range for the accelerometer
     * @param range New range for the accelerometer
     */
    void set_accel_range(mpu6050_accel_range_t range);
private:
    Adafruit_MPU6050 mpu;
};



#endif