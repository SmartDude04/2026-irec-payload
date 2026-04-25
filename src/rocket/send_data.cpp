#include <rocket.h>

bool send_data(const uint8_t *data, const uint8_t len, const char *str_data)
{
    bool radio_success = true;
    bool sd_success = true;
    bool serial_success = true;
    if constexpr (RADIO)
    {
        radio_success = radio.send(data, len);
    }
    if constexpr (SD_CARD)
    {
        sd_success = sd.write(str_data);
    }
    if constexpr (DEBUG)
    {
        serial_success = Serial.println(str_data);
    }
    
    return radio_success && sd_success && serial_success;
}