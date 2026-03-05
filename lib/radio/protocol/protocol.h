#ifndef INC_2026_IREC_PAYLOAD_PROTOCOL_H
#define INC_2026_IREC_PAYLOAD_PROTOCOL_H

#include <Arduino.h>

typedef struct
{
    float temperature;
    float humidity;
    float pressure;
    float CO2;
    float altitude;
} radio_data;

/**
 * Converts a radio_data struct into a stream of raw bytes
 * @param data radio_data struct whose data is converted into raw bytes
 * @param message Location of the raw bytes to convert into
 * @param message_length Should be given as the length of the message array, but will be changed to the size of the
 * resulting byte array for the radio data. It will also be checked to make sure it is big enough to store the data
 */
void pack_radio_data(const radio_data &data, uint8_t *message, size_t &message_length);

/**
 * Converts a stream of raw bytes to a radio_data struct
 * @param data radio_data struct that will store the data from the message
 * @param message Raw bytes to convert from
 * @param message_length Length of the raw bytes. Will simply by checked to make sure its size is at least as large as
 * a radio_data struct
 */
void unpack_radio_data(radio_data &data, const uint8_t *message, size_t message_length);

#endif