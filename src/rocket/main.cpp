#include <Arduino.h>
#include <radio.h>
#include <pressure.h>
#include <scd.h>
#include <protocol/protocol.h>

radio radio;
pressure pressure;
scd scd;

void setup()
{
    // Begin serial output
    Serial.begin(115200);
    while (!Serial) { delay(1); }
    
    const uint8_t radio_init = radio.init();
    const uint8_t pressure_init = pressure.init();
    const uint8_t scd_init = scd.init();
    
    // Initalize the radio
    if (!radio_init)
    {
        Serial.println("ERROR: Radio failed to initialize");
    }
    else
    {
        Serial.println("Radio initialized");
    }
    
    if (!pressure_init)
    {
        Serial.println("ERROR: Pressure sensor failed to initialize");
    }
    else
    {
        Serial.println("Pressure sensor initialized");
    }
    
    if (!scd_init)
    {
        Serial.println("ERROR: SCD30 failed to initialize");
    }
    else
    {
        scd.set_measurement_interval(2);
        Serial.println("SCD30 initialized");
    }
    
    if (!(radio_init && pressure_init && scd_init))
    {
        while (true) {}
    }
}

void loop()
{
    uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t message_length = sizeof(message);
    if (radio.receive(message, message_length))
    {
        if (message[0] == 0x01)
        {
            Serial.println("Message Received! Packing and sending sensor data");
            // Wait until data is ready
            while (!scd::data_ready()) {}
    
            scd_data data;
            scd.read_data(data);
            radio_data radio_send_data;
            radio_send_data.temperature = data.temperature;
            radio_send_data.humidity = data.humidity;
            radio_send_data.CO2 = data.CO2;
            radio_send_data.pressure = pressure.get_pressure_psi();
            uint8_t message_send[sizeof(radio_send_data)];
            size_t message_length_send = sizeof(radio_send_data);
            pack_radio_data(radio_send_data, message_send, message_length_send);
            radio.send(message_send, message_length_send);
            
            Serial.println("Message Sent! Waiting for message to be received...");
        }
    }
}