#include "protocol.h"

void pack_radio_data(const radio_data &data, uint8_t *message, size_t &message_length)
{
    if (message_length < sizeof(radio_data))
    {
        return;
    }
    
    auto *buffer = reinterpret_cast<const uint8_t*>(&data);
    memcpy(message, buffer, sizeof(radio_data));
    message_length = sizeof(radio_data);
}

void unpack_radio_data(radio_data &data, const uint8_t *message, size_t message_length)
{
    if (message_length < sizeof(radio_data))
    {
        return;
    }
    
    memcpy(&data, message, sizeof(radio_data));
}
