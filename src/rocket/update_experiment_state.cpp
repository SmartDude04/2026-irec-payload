#include <rocket.h>
#include <payloads/string_payload.h>

void update_experiment_state()
{
    uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];
    
    if (!experiment_started && acceleration_norm < EXPERIMENT_ACCEL_THRESHOLD && altitude_m >= EXPERIMENT_ALTITUDE_THRESHOLD)
    {
        // Start the experiment!
        experiment_started = true;
        servo.write(180);
        
        string_payload payload;
        strcpy(payload.message, "Experiment started!");
        
        const uint8_t len = protocol.encode(STRING_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
        
        if (len > 0)
        {
            send_data(radio_packet, len, "Experiment started!");
        }
    }
    
    if constexpr (!DEBUG)
    {
        if (experiment_started && acceleration_norm > ACCELERATION_GROUND - 1.0)
        {
            experiment_started = false;
            servo.write(0);
            
            string_payload payload;
            strcpy(payload.message, "Experiment ended!");
            
            const uint8_t len = protocol.encode(STRING_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
            
            if (len > 0)
            {
                send_data(radio_packet, len, "Experiment ended!");
            }
        }
    }
}