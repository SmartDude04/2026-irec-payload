#include "protocol.h"

#include <RH_RF95.h>

protocol::protocol() : payload_num(0) {}

uint8_t protocol::encode(const uint8_t type, const uint8_t *payload, const uint8_t length, uint8_t *bytes)
{
    // Make sure the payload length is within range
    if (length > MAX_PAYLOAD_LENGTH)
    {
        return 0;
    }

    // Build the bytes variable piece-by-piece
    uint8_t index = 0;
    bytes[index++] = START_BYTE;
    bytes[index++] = type;
    bytes[index++] = payload_num++;
    bytes[index++] = length;
    memcpy(bytes + index, payload, length);
    index += length;

    // Compute and append on the checksum
    const uint8_t checksum = compute_checksum(bytes, index);
    bytes[index++] = checksum;

    // Return the length of the bytes
    return index;
}

bool protocol::decode(const uint8_t *const bytes, const uint8_t len, packet &dest_packet)
{
    decode_state cur_state = WAIT_START_BYTE;
    uint8_t checksum = 0;
    uint8_t payload_index = 0;
    packet cur_packet = {};

    for (uint8_t i = 0; i < len && i < RH_RF95_MAX_MESSAGE_LEN; i++)
    {
        switch (cur_state)
        {
        case WAIT_START_BYTE: {
            if (bytes[i] == START_BYTE)
            {
                cur_state = READ_TYPE;
                checksum ^= bytes[i];
                break;
            }
            return false;
        }
        case READ_TYPE: {
            cur_packet.type = bytes[i];
            checksum ^= bytes[i];
            cur_state = READ_PAYLOAD_NUM;
            break;
        }
        case READ_PAYLOAD_NUM: {
            cur_packet.payload_num = bytes[i];
            checksum ^= bytes[i];
            cur_state = READ_LENGTH;
            break;
        }
        case READ_LENGTH: {
            cur_packet.length = bytes[i];
            checksum ^= bytes[i];
            cur_state = READ_PAYLOAD;
            break;
        }
        case READ_PAYLOAD: {
            cur_packet.payload[payload_index++] = bytes[i];
            checksum ^= bytes[i];
            if (payload_index == cur_packet.length) cur_state = READ_CHECKSUM;
            break;
        }
        case READ_CHECKSUM: {
            if (bytes[i] == checksum)
            {
                dest_packet = cur_packet;
                return true;
            }
            return false;
        }
        }
    }
    // Shouldn't get to here if the bytes are correct!
    return false;
}

uint8_t protocol::compute_checksum(const uint8_t *buffer, const uint8_t length)
{
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        checksum ^= buffer[i];
    }
    return checksum;
}
