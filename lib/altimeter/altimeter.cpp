#include "altimeter.h"

#define SEA_LEVEL_PRESSURE 1017.79

bool altimeter_t::init()
{
    bool success = false;
    for (int i = 0; i < 5; i++)
    {
        if (bmp388.begin_I2C())
        {
            success = true;
            break;
        }
    }
    
    if (!success)
    {
        return false;
    }
    
    bmp388.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
    bmp388.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp388.setOutputDataRate(BMP3_ODR_200_HZ);
    bmp388.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
    
    return true;
}

float altimeter_t::get_altitude()
{
    return bmp388.readAltitude(SEA_LEVEL_PRESSURE);
}
