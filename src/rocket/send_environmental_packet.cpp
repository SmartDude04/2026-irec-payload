#include <rocket.h>

#include <payloads/most_environmental.h>
#include <payloads/all_environmental.h>

uint32_t last_scd_data_ready = 0;
constexpr uint32_t SCD_DATA_READY_DELAY = 2000;

void send_environmental_packet()
{
    uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];
    
    // If we have C02 and temp data, then send it. Otherwise, don't
    if (millis() - last_scd_data_ready >= SCD_DATA_READY_DELAY && scd.data_ready())
    {
        last_scd_data_ready = millis();
        all_environmental_payload payload;
        payload.pressure_hpa = pressure.get_pressure_hpa();
        payload.altitude_m = altimeter.get_altitude();
        payload.acceleration_g = static_cast<float>(acceleration_norm);
        
        scd_data_t cur_scd_values;
        if (scd.read_data(cur_scd_values))
        {
            payload.CO2 = cur_scd_values.CO2;
            payload.temperature = cur_scd_values.temperature;
            payload.humidity = cur_scd_values.humidity;
            
            const uint8_t packet_length = protocol.encode(ALL_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
            if (packet_length)
            {
                String message = "Time: ";
                message += String(static_cast<double>(millis()) / 1000.0);
                message += " s, Altitude: ";
                message += String(payload.altitude_m);
                message += " m, Pressure: ";
                message += String(payload.pressure_hpa);
                message += " hPa, Acceleration: ";
                message += String(payload.acceleration_g);
                message += ", CO2: ";
                message += String(payload.CO2);
                message += " ppm, Temperature: ";
                message += String(payload.temperature);
                message += " C, Humidity: ";
                message += String(payload.humidity);
                message += " %";
                send_data(radio_packet, packet_length, message.c_str());
            }
            else
            {
                // Something happened, set them to zero but log it
                if constexpr (DEBUG)
                {
                    Serial.println("ERROR: Failed to encode the environmental data packet");
                }
                payload.CO2 = 0.0f;
                payload.temperature = 0.0f;
                payload.humidity = 0.0f;
            }
        }
        else
        {
            if constexpr (DEBUG)
            {
                Serial.println("ERROR: Failed to read SCD30 data even though it said it was ready");
            }
        }
    }
    else // SCD30 data isn't ready
    {
        most_environmental_payload payload;
        payload.pressure_hpa = pressure.get_pressure_hpa();
        payload.altitude_m = altimeter.get_altitude();
        payload.acceleration_g = static_cast<float>(acceleration_norm);
        
        const uint8_t len = protocol.encode(MOST_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
        
        if (len > 0)
        {
            String message = "Time: ";
            message += String(static_cast<double>(millis()) / 1000.0);
            message += " s, Altitude: ";
            message += String(payload.altitude_m);
            message += " m, Pressure: ";
            message += String(payload.pressure_hpa);
            message += " hPa, Acceleration: ";
            message += String(payload.acceleration_g);
            message += " g";
            send_data(radio_packet, len, message.c_str());
        }
        else
        {
            if constexpr (DEBUG)
            {
                Serial.println("ERROR: Failed to encode the environmental data packet");
            }
        }
    }
}