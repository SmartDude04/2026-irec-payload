#include <rocket.h>

void update_acceleration_altitude()
{
    constexpr int old_amt = 5;
    constexpr int new_amt = 5;
    
    float ax, ay, az;
    accelerometer.get_acceleration(ax, ay, az);
    const float norm_accel = sqrt(ax * ax + ay * ay + az * az);
    
    if (acceleration_norm == -1)
    {
        acceleration_norm = norm_accel;
    }
    else
    {
        acceleration_norm = (acceleration_norm * old_amt + norm_accel * new_amt) / (old_amt + new_amt);
    }
    
    altitude_m = altimeter.get_altitude();
}