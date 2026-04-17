#include "accelerometer.h"

accelerometer::accelerometer() = default;

bool accelerometer::init()
{
    bool success = false;
    for (int i = 0; i < 5; i++)
    {
        if (lsm6dsl.begin_I2C())
        {
            success = true;
            break;
        }
        delay(100);
    }
    
    if (!success)
    {
        return false;
    }
    
    // Set a fast data rate for the accelerometer and the slowest for the gyro as we won't be using it
    lsm6dsl.setAccelDataRate(LSM6DS_RATE_833_HZ);
    lsm6dsl.setGyroDataRate(LSM6DS_RATE_26_HZ);
    
    lsm6dsl.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
    
    return true;
}

bool accelerometer::get_acceleration(float &x, float &y, float &z)
{
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    const bool ret = lsm6dsl.getEvent(&accel, &gyro, &temp);
    x = accel.acceleration.x;
    y = accel.acceleration.y;
    z = accel.acceleration.z;
    return ret;
}
