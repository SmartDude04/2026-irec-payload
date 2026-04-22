#include <rocket.h>

void avg_acceleration()
{
    constexpr int old_amt = 5;
    constexpr int new_amt = 5;
    
    float ax, ay, az;
    accelerometer.get_acceleration(ax, ay, az);
    const float norm_accel = sqrt(ax * ax + ay * ay + az * az);
    
    acceleration_norm = (acceleration_norm * old_amt + norm_accel * new_amt) / (old_amt + new_amt);
}