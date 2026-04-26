#include <rocket.h>

void update_acceleration_altitude()
{
    constexpr int old_amt = 5;
    constexpr int new_amt = 5;
    
    float ax, ay, az;
    bool success = accelerometer.get_acceleration(ax, ay, az);
    const float norm_accel = sqrt(ax * ax + ay * ay + az * az);
    
    if (!success)
    {
        if constexpr (DEBUG)
        {
            Serial.println("WARNING: Bad accelerometer read, attempting I2C recovery");
        }
        recover_i2c_bus();
        return;
    }
    
    altitude_m = altimeter.get_altitude();
    
    if (acceleration_norm == -1)
    {
        acceleration_norm = norm_accel;
    }
    else
    {
        acceleration_norm = (acceleration_norm * old_amt + norm_accel * new_amt) / (old_amt + new_amt);
    }
    
}