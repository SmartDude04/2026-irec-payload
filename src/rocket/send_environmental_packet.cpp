#include <rocket.h>

#include <payloads/most_environmental.h>
#include <payloads/all_environmental.h>

void send_environmental_packet()
{
    uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];
    
    // If we have C02 and temp data, then send it. Otherwise, don't
    if (scd_t::data_ready())
    {
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
            
            Serial.print("Altitude: ");
            Serial.print(payload.altitude_m);
            Serial.print(" m, Pressure: ");
            Serial.print(payload.pressure_hpa);
            Serial.print(" hPa, Acceleration: ");
            Serial.print(payload.acceleration_g);
            Serial.print(" g CO2: ");
            Serial.print(payload.CO2);
            Serial.print(" ppm, Temperature: ");
            Serial.print(payload.temperature);
            Serial.print(" C, Humidity: ");
            Serial.println(payload.humidity);
            
            if (const uint8_t ret = protocol.encode(ALL_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet)))
            {
                radio.send(radio_packet, ret);
            }
            else
            {
                // Something happened, set them to zero but log it
                Serial.println("ERROR: Failed to encode the environmental data packet");
                payload.CO2 = 0.0f;
                payload.temperature = 0.0f;
                payload.humidity = 0.0f;
            }
            
            const uint8_t packet_length = protocol.encode(ALL_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
            
            if (packet_length > 0)
            {
                radio.send(radio_packet, packet_length);
            }
            else
            {
                Serial.println("ERROR: Failed to encode the environmental data packet");
            }
        }
        else
        {
            Serial.println("ERROR: Failed to read SCD30 data even though it said it was ready");
        }
    }
    else // SCD30 data isn't ready
    {
        most_environmental_payload payload;
        payload.pressure_hpa = pressure.get_pressure_hpa();
        payload.altitude_m = altimeter.get_altitude();
        payload.acceleration_g = acceleration_norm;
        
        Serial.print("Altitude: ");
        Serial.print(payload.altitude_m);
        Serial.print(" m, Pressure: ");
        Serial.print(payload.pressure_hpa);
        Serial.print(" hPa, Acceleration: ");
        Serial.println(payload.acceleration_g);
        
        const uint8_t len = protocol.encode(MOST_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
        
        if (len > 0)
        {
            radio.send(radio_packet, len);
        }
        else
        {
            Serial.println("ERROR: Failed to encode the environmental data packet");
        }
    }
}