#include "accelerometer.h"

accelerometer::accelerometer() = default;

bool accelerometer::init()
{
    Wire.begin();
    // Check if something exists on address 0x68 first
    mpu.initialize();
    delay(10);
    return mpu.testConnection();
}

void accelerometer::read_data(accelerometer_data &data)
{
    mpu.getMotion6(&data.ax, &data.ay, &data.az, &data.gx, &data.gy, &data.gz);
}

uint8_t accelerometer::get_accel_range()
{
    return mpu.getFullScaleAccelRange();
}

void accelerometer::set_accel_range(const uint8_t range)
{
    mpu.setFullScaleAccelRange(range);
}
