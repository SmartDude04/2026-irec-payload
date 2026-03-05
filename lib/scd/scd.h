#ifndef INC_2026_IREC_PAYLOAD_SCD_H
#define INC_2026_IREC_PAYLOAD_SCD_H

#include <stdint.h>
#include <Adafruit_SCD30.h>

typedef struct
{
    float temperature;
    float humidity;
    float CO2;
} scd_data;

class scd
{
public:
    scd();

    /**
     * Attempts to find and initialize the SCD30 sensor at I2C address x61 with the following pinouts:
     * - 3.3V -> VIN
     * - GND ->  GND
     * - SCL ->  SCL
     * - SDA ->  SDA
     * - RDY ->  9
     * @return True if successfully found and initialized the sensor; false otherwise
     */
    bool init();

    /**
     * Gets the current measurement interval for the sensor
     * @return The minimum time between measurements, in seconds
     */
    uint16_t get_measurement_interval();

    /**
     * Attempts to set the time between sensor values being ready
     * @param interval The time between measurements in seconds. Must be from 2-1800 seconds. The default value set on
     * sensor initialization is 2 seconds.
     * @return True if successfully set; false otherwise
     */
    bool set_measurement_interval(uint16_t interval);

    /**
     * Attempts to set the altiude offset for the sensor. This value is saved to the SCD30's internal storage and is
     * reloaded on sensor power up.
     * @param altitude Current altitude, in meters
     * @return True on success; false otherwise
     */
    bool set_altitude_offset(uint16_t altitude);

    /**
     * Returns if data is ready to be collected from the sensor
     * @return True if data ready; false otherwise
     */
    static bool data_ready();

    /**
     * Attempts to read data from the sensor and put it into the data parameter
     * @param data Reference to a struct whose data will be overridden by the data just read, if successful
     * @return True if successfully read data from the sensor; false otherwise
     */
    bool read_data(scd_data &data);
    
    /**
     * Attempts to calibrate the CO2 sensor with a known environmental reading. This value is saved to the SCD30's
     * internal storage and is reloaded on sensor power up. Setting a reference value and forcing recalibration will
     * override any previous automatic self-calibration.
     * 
     * WARNING: Only attempt a calibration if you know what you are doing and have an accurate measurement!
     * @param altitude Current altitude, in meters, of the sensor from sea level
     * @param true_co2 The calibration reference value in ppm from 400-2000 ppm.
     * @return True if successfully calibrated; false otherwise
     */
    bool calibrate(uint16_t altitude, uint16_t true_co2);
private:
    Adafruit_SCD30 scd30;
};


#endif