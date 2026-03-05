#include <Arduino.h>
#include "radio/radio.h"

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
    if (radio.message_available())
    {
        Serial.println(radio.receive());
        Serial.println("RSSI: " + String(radio.last_rssi()));
    }
    // const bool ret = radio.send("Hello!");
    // Serial.println(ret ? "Sent" : "Failed to send");
    // delay(1000);
}