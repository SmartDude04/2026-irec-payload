#include <Arduino.h>
#include <radio.h>

#include "protocol/protocol.h"

radio radio;

void setup()
{
    // Begin serial output
    Serial.begin(115200);
    while (!Serial) { delay(1); }
    
    // Initalize the radio
    if (!radio.init())
    {
        Serial.println("ERROR: Radio failed to initialize");
        while (true) {}
    }
    
    Serial.println("Radio initialized");
}

void loop()
{
    uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
    
    message[0] = 0x01;
    radio.send(message, 1);
    
    while (!radio.message_available()) {}
    
    uint8_t message_length = sizeof(message);
    if (radio.receive(message, message_length))
    {
        radio_data data;
        unpack_radio_data(data, message, message_length);
        Serial.println("Message Received!");
        Serial.println("\tTemperature: " + String(data.temperature));
        Serial.println("\tHumidity: " + String(data.humidity));
        Serial.println("\tCO2: " + String(data.CO2));
        Serial.println("\tPressure: " + String(data.pressure));
    }
    
    delay(1000);
}