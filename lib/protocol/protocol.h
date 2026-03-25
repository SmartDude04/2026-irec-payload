#ifndef INC_2026_IREC_PAYLOAD_PROTOCOL_H
#define INC_2026_IREC_PAYLOAD_PROTOCOL_H

#include <Arduino.h>

class protocol
{
public:
    // Byte that starts all communication
    static constexpr uint8_t START_BYTE = 0x66;
    
    // Maximum length for a payload to be sent over the radio
    static constexpr uint8_t MAX_PAYLOAD_LENGTH = 192;
    
    // Data and metadata about a radio packet
    struct packet
    {
        // Number representing the type of the radio packet
        uint8_t type;
        
        // The number for this payload; if a number is skipped a transmission is skipped
        uint16_t payload_num;
        
        // Length of the payload
        uint8_t length;
        
        // Payload
        uint8_t payload[MAX_PAYLOAD_LENGTH];
    };
    
    // Size of the storage container for the packet
    static constexpr uint8_t MIN_BYTES_FOR_PACKET = MAX_PAYLOAD_LENGTH + 6;

    /**
     * Initialize the protocol object. Must be done before attempting to transmit data!
     */
    protocol();

    /**
     * 
     * @param type Number representing the type of data being sent over the radio. TODO: Make file detailing types and structs for radio types
     * @param payload Payload to send over the radio
     * @param payload_length Length of the payload
     * @param bytes Bytes where the payload will be stored. Should be of size at least RH_RF95_MAX_MESSAGE_LEN
     * @param bytes_length length of the bytes array
     * @return Number of bytes output in the buffer (i.e. size of packet to be sent over radio), or zero if unsuccessful (likely due to size of the bytes)
     */
    uint8_t encode(uint8_t type, const uint8_t *payload, uint8_t payload_length, uint8_t *bytes, uint16_t bytes_length);

    /**
     * 
     * @param bytes Raw radio data received
     * @param len Length of the radio data
     * @param dest_packet Packet struct where the data will be stored
     * @return True if the calculated checksum matches the given one. If false, packet loss / interference has
     * occurred and the data should not be considered trustworthy
     */
    static bool decode(const uint8_t *bytes, uint8_t len, packet &dest_packet);
private:
    uint16_t payload_num;
    
    // Stores the finite state of the decoding process. Used to guarantee the proper decoding of a message
    enum decode_state
    {
        WAIT_START_BYTE,
        READ_TYPE,
        READ_PAYLOAD_NUM,
        READ_LENGTH,
        READ_PAYLOAD,
        READ_CHECKSUM
    };

    /**
     * Computes a checksum for the message
     * @param buffer Buffer to compute the checksum
     * @param length Length of the message
     * @return The checksum for this buffer
     */
    static uint8_t compute_checksum(const uint8_t *buffer, uint8_t length);
};

#endif