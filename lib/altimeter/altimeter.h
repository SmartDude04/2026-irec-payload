#ifndef INC_2026_IREC_PAYLOAD_ALTIMETER_H
#define INC_2026_IREC_PAYLOAD_ALTIMETER_H
#include <Adafruit_BMP3XX.h>

class altimeter_t
{
public:
    /**
     * Attempts to initialize the altimeter with the sea level pressure defined in altimeter.cpp
     * @return True if successfully initialized; false otherwise
     */
    bool init();

    /**
     * With a proper setting of the sea level pressure in altimeter.cpp, returns the altitude
     * in meters
     * @return The altitude, in meters
     */
    float get_altitude();
private:
    Adafruit_BMP3XX bmp388;
};



#endif