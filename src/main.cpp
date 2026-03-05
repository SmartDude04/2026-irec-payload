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
    
    // Initalize the radio
    if (!radio.init())
    {
        Serial.println("ERROR: Radio failed to initialize");
        while (true) {}
    }
    Serial.println("Radio initialized");
    
    if (!pressure.init())
    {
        Serial.println("ERROR: Pressure sensor failed to initialize");
        while (true) {}
    }
    Serial.println("Pressure sensor initialized");
    
    if (!scd.init())
    {
        Serial.println("ERROR: SCD30 failed to initialize");
        while (true) {}
    }
    scd.set_measurement_interval(2);
    Serial.println("SCD30 initialized");
}

void loop()
{
    scd_data data;
    if (scd.read_data(data))
    {
        radio_data rd;
        rd.altitude = 0;
        rd.pressure = pressure.get_pressure_hpa();
        rd.temperature = data.temperature;
        rd.humidity = data.humidity;
        rd.CO2 = data.CO2;

        uint8_t message[RH_RF95_MAX_MESSAGE_LEN + 1];
        size_t message_length = sizeof(message);
        pack_radio_data(rd, message, message_length);
        
        rd = {};
        unpack_radio_data(rd, message, message_length);
        
        Serial.println("Data: ");
        Serial.println("Altitude: " + String(rd.altitude));
        Serial.println("Pressure: " + String(rd.pressure));
        Serial.println("Temperature: " + String(rd.temperature));
        Serial.println("Humidity: " + String(rd.humidity));
        Serial.println("CO2: " + String(rd.CO2));
    }
}