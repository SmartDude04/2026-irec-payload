#include <rocket.h>
#include <payloads/arm_payload.h>

void handle_incoming_radio_messages()
{
    uint8_t incoming_radio_message[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t message_length = sizeof(incoming_radio_message);
    
    if (!radio.message_available())
    {
        return;
    }
    Serial.println("Packet available");
    
    if (!radio.receive(incoming_radio_message, message_length))
    {
        return;
    }
    Serial.println("Packet successfully received");
    
    protocol_t::packet packet = {};
    if (!protocol_t::decode(incoming_radio_message, message_length, packet))
    {
        // Something happened (bad checksum, failed to decode, etc.); print error and return
        Serial.println("ERROR: Failed to decode packet. RSSI: " + String(radio.last_rssi()));
    }
    
    switch (packet.type)
    {
    case ARM_PAYLOAD_TYPE: {
        const bool do_arm = reinterpret_cast<arm_payload*>(packet.payload)->arm;
        Serial.println("Received arm signal from ground station with arm value " + String(do_arm));
        if (do_arm)
        {
            if (!armed) // If we aren't armed, do so
            {
                armed = true;
                experiment_started = false;
                servo.write(0);
            }
            send_arm_ack(true);
        }
        else
        {
            armed = false;
            experiment_started = false;
            servo.write(180);
            send_arm_ack(false);
        }
        break;
    }
    default: {
        // Payload type not recognized / not available for reception; print error
        Serial.println("ERROR: Received packet with unknown payload type. RSSI: " + String(radio.last_rssi()));
    }
    }
}
