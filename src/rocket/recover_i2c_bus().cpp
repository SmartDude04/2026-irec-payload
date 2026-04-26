#include <rocket.h>

void recover_i2c_bus()
{
    Wire.end();
    
    pinMode(SCL, OUTPUT);
    pinMode(SDA, INPUT_PULLUP);
    
    // Bit-bang up to 9 clock pulses on SCL to free a stuck slave
    for (int i = 0; i < 9; i++)
    {
        digitalWrite(SCL, LOW);
        delayMicroseconds(5);
        digitalWrite(SCL, HIGH);
        delayMicroseconds(5);
        
        // If SDA is released, the slave is unstuck
        if (digitalRead(SDA) == HIGH)
        {
            break;
        }
    }
    
    Wire.begin();
}