#include "scd.h"

#define RDY_PIN 9

scd_t::scd_t() = default;

bool scd_t::init()
{
    pinMode(RDY_PIN, INPUT);
    delay(10);

    // Try top start the sensor 5 times
    for (int i = 0; i < 5; i++)
    {
        if (scd30.begin())
        {
            return true;
        }
        delay(100);
    }
    return false;
}

uint16_t scd_t::get_measurement_interval()
{
    return scd30.getMeasurementInterval();
}

bool scd_t::set_measurement_interval(const uint16_t interval)
{
    return scd30.setMeasurementInterval(interval);
}

bool scd_t::set_altitude_offset(const uint16_t altitude)
{
    return scd30.setAltitudeOffset(altitude);
}

bool scd_t::data_ready()
{
    return scd30.dataReady();
}

bool scd_t::read_data(scd_data_t &data)
{
    // TODO: There is an altitude offset that we should use then we get the altimeter
    if (data_ready())
    {
        const bool ret = scd30.read();
        if (!ret)
        {
            return false;
        }
        
        data.temperature = scd30.temperature;
        data.humidity = scd30.relative_humidity;
        data.CO2 = scd30.CO2;
        return true;
    }
    return false;
}

bool scd_t::calibrate(const uint16_t altitude, const uint16_t true_co2)
{
    const uint16_t cur_alt = scd30.getAltitudeOffset();
    bool ret = scd30.setAltitudeOffset(altitude);
    if (!ret)
    {
        return false;
    }
    
    ret = scd30.forceRecalibrationWithReference(true_co2);
    scd30.setAltitudeOffset(cur_alt);
    return ret;
}
