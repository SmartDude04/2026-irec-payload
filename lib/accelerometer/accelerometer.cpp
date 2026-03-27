#include "accelerometer.h"

#include <Adafruit_Sensor.h>

accelerometer::accelerometer() = default;

bool accelerometer::init()
{
    // Check if something exists on address 0x68 first
    return mpu.begin();
}

void accelerometer::read_data(accelerometer_data &data)
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    data = {
        .ax = a.acceleration.x,
        .ay = a.acceleration.y,
        .az = a.acceleration.z,
        .gx = g.gyro.x,
        .gy = g.gyro.y,
        .gz = g.gyro.z
    };
}

mpu6050_accel_range_t accelerometer::get_accel_range()
{
    return mpu.getAccelerometerRange();
}

void accelerometer::set_accel_range(const mpu6050_accel_range_t range)
{
    mpu.setAccelerometerRange(range);
}
