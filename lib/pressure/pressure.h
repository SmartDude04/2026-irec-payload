#ifndef INC_2026_IREC_PAYLOAD_PRESSURE_H
#define INC_2026_IREC_PAYLOAD_PRESSURE_H

#include <Adafruit_MPRLS.h>

class pressure_t
{
public:
    pressure_t();

    /**
     * Attempts to find and initialize the MPRLS Pressure Sensor at I2C address 0x18 with pinouts:
     * - 3.3V-5V -> VIN
     * - GND ->  GND
     * - SDA ->  SDA
     * - SCL ->  SCL
     * - EOC ->  7
     * - RST ->  8
     * @return True if successfully found the sensor and initialized it; false otherwise
     */
    bool init();

    /**
     * Performs a hardware reset of the pressure sensor
     */
    static void reset();

    /**
     * Returns the pressure in hectopascals (1 hPa = 100 Pa)
     * @return Pressure in hectopascals or NAN on failure
     */
    float get_pressure_hpa();

    /**
     * Returns the pressure in PSI
     * @return Pressure in PSI or NAN on failure
     */
    float get_pressure_psi();
    
    
private:
    Adafruit_MPRLS mprls;
};


#endif