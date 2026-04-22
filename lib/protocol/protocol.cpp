#include "protocol.h"

#include <RH_RF95.h>

protocol_t::protocol_t() : payload_num(0) {}

uint8_t protocol_t::encode(const uint8_t type, const uint8_t *payload, const uint8_t payload_length, uint8_t *bytes, const uint16_t bytes_length)
{
    if (bytes == nullptr || (payload == nullptr && payload_length > 0)) return 0;
    
    // Make sure the payload length is within range
    if (payload_length > MAX_PAYLOAD_LENGTH)
    {
        return 0;
    }
    const uint16_t needed_length = payload_length + 6;
    if (bytes_length < needed_length)
    {
        return 0;
    }

    // Build the bytes variable piece-by-piece
    uint8_t index = 0;
    bytes[index++] = START_BYTE;
    bytes[index++] = type;
    
    // Transmit both bytes for payload_num
    bytes[index++] = payload_num >> 8;
    bytes[index++] = payload_num & 0xFF;
    payload_num++;
    
    bytes[index++] = payload_length;
    if (payload_length > 0)
    {
        memcpy(bytes + index, payload, payload_length);
    }
    index += payload_length;

    // Compute and append on the checksum
    const uint8_t checksum = compute_checksum(bytes, index);
    bytes[index++] = checksum;

    // Return the length of the bytes
    return index;
}

bool protocol_t::decode(const uint8_t *const bytes, const uint8_t len, packet &dest_packet)
{
    if (bytes == nullptr) return false;
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
            // Make sure to get both bytes for payload_num
            if (i + 1 >= len) return false; // Check for out-of-bounds
            cur_packet.payload_num = (bytes[i] << 8) | bytes[i + 1];
            checksum ^= bytes[i] ^ bytes[i + 1];
            i++;
            cur_state = READ_LENGTH;
            break;
        }
        case READ_LENGTH: {
            cur_packet.length = bytes[i];
            checksum ^= bytes[i];
            
            // Make sure we don't overflow the payload
            if (cur_packet.length > MAX_PAYLOAD_LENGTH)
            {
                return false;
            }
            
            // Check for zero-length payloads
            if (cur_packet.length == 0)
            {
                cur_state = READ_CHECKSUM;
                break;
            }
            else
            {
                cur_state = READ_PAYLOAD;
                break;
            }
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
            else
            {
                return false;
            }
        }
        }
    }
    // Shouldn't get to here if the bytes are correct!
    return false;
}

uint8_t protocol_t::compute_checksum(const uint8_t *buffer, const uint8_t length)
{
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++)
    {
        checksum ^= buffer[i];
    }
    return checksum;
}
