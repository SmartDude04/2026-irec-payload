#include <rocket.h>
#include <payloads/arm_payload.h>

bool send_arm_ack(const bool arm_state)
{
    uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];
    arm_payload ack = { arm_state };
    
    const uint8_t len = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&ack), sizeof(ack), radio_packet, sizeof(radio_packet));
    
    if (len == 0)
    {
        return false;
    }
    
    return send_data(radio_packet, len, "Acknowledged arm signal");
}