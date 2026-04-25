// #include <Arduino.h>
// #include <radio.h>
// #include <protocol.h>
// #include <payloads/arm_payload.h>
// #include <payloads/pressure_payload.h>
// #include <payloads/scd_payload.h>
// #include <payloads/string_payload.h>
// #include <payloads/most_environmental.h>
// #include <payloads/all_environmental.h>
//
// radio_t radio;
// protocol_t protocol;
//
// void setup()
// {
//     // Begin serial output
//     Serial.begin(115200);
//     while (!Serial) { delay(1); }
//
//     // Initalize the radio
//     if (!radio.init())
//     {
//         Serial.println("ERROR: Radio failed to initialize");
//         while (true)
//         {
//         }
//     }
//
//     Serial.println("Radio initialized");
// }
//
// void loop()
// {
//     // Check for any incoming messages
//     if (radio.message_available())
//     {
//         uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
//         uint8_t message_length = sizeof(message);
//         if (radio.receive(message, message_length))
//         {
//             protocol_t::packet packet = {};
//             const bool decode_success = protocol_t::decode(message, message_length, packet);
//             if (!decode_success)
//             {
//                 Serial.println(
//                     "ERROR: Failed to decode packet. Checksum Doesn't Match. RSSI: " + String(radio.last_rssi()));
//                 return;
//             }
//
//             switch (packet.type)
//             {
//             case ARM_PAYLOAD_TYPE: {
//                 const arm_payload payload = *(reinterpret_cast<arm_payload*>(packet.payload));
//                 if (payload.arm)
//                 {
//                     Serial.println("Received arm signal from flight computer");
//                 }
//                 else
//                 {
//                     Serial.println("Received disarm signal from flight computer");
//                 }
//                 break;
//             }
//             case PRESSURE_PAYLOAD_TYPE: {
//                 const pressure_payload payload = *(reinterpret_cast<pressure_payload*>(packet.payload));
//                 Serial.print("Pressure: ");
//                 Serial.print(payload.pressure_hpa);
//                 Serial.println(" hPa");
//                 break;
//             }
//             case SCD_PAYLOAD_TYPE: {
//                 const scd_payload payload = *(reinterpret_cast<scd_payload*>(packet.payload));
//                 Serial.print("CO2: ");
//                 Serial.print(payload.CO2);
//                 Serial.print(" ppm, Temperature: ");
//                 Serial.print(payload.temperature);
//                 Serial.print(" C, Humidity: ");
//                 Serial.print(payload.humidity);
//                 Serial.println(" %");
//                 break;
//             }
//             case STRING_PAYLOAD_TYPE: {
//                 const string_payload payload = *(reinterpret_cast<string_payload*>(packet.payload));
//                 Serial.println("Received message: " + String(payload.message));
//                 break;
//             }
//             case MOST_ENVIRONMENTAL_TYPE: {
//                 const most_environmental_payload payload = *(reinterpret_cast<most_environmental_payload*>(packet.payload));
//                 Serial.print("Pressure: ");
//                 Serial.print(payload.pressure_hpa);
//                 Serial.print(" hPa, Altitude: ");
//                 Serial.print(payload.altitude_m);
//                 Serial.print(" m, Acceleration: ");
//                 Serial.print(payload.acceleration_g);
//                 Serial.println(" g");
//                 break;
//             }
//             case ALL_ENVIRONMENTAL_TYPE: {
//                 const all_environmental_payload payload = *(reinterpret_cast<all_environmental_payload*>(packet.payload));
//                 Serial.print("Pressure: ");
//                 Serial.print(payload.pressure_hpa);
//                 Serial.print(" hPa, Altitude: ");
//                 Serial.print(payload.altitude_m);
//                 Serial.print(" m, Acceleration: ");
//                 Serial.print(payload.acceleration_g);
//                 Serial.print(" g, CO2: ");
//                 Serial.print(payload.CO2);
//                 Serial.print(" ppm, Temperature: ");
//                 Serial.print(payload.temperature);
//                 Serial.print(" C, Humidity: ");
//                 Serial.print(payload.humidity);
//                 Serial.println(" %");
//                 break;
//             }
//             default: {
//                 Serial.println("Received unknown packet type that passed checksum");
//             }
//             }
//         }
//     }
//     
//     // Check for any arm or disarm signals given from the ground station via the serial terminal
//     if (Serial.available())
//     {
//         const int c = Serial.read();
//         if (c == 'a')
//         {
//             Serial.println("To arm, press \'a\' once more in the next second...");
//             delay(1000);
//             if (!(Serial.available() && Serial.read() == 'a')) { Serial.println("Aborted arm process"); return; }
//             Serial.println("Sending arm signal and waiting for a response");
//             
//             uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
//             arm_payload arm = { true };
//             const uint8_t message_length = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t*>(&arm), sizeof(arm), message, sizeof(message));
//             if (message_length == 0)
//             {
//                 Serial.println("ERROR: Failed to encode arm signal. Try again?");
//                 return;
//             }
//             radio.send(message, message_length);
//         }
//         else if (c == 'd')
//         {
//             Serial.println("To disarm, press \'d\' once more in the next second...");
//             delay(1000);
//             if (!(Serial.available() && Serial.read() == 'd')) { Serial.println("Aborted disarm process"); return; }
//             Serial.println("Sending disarm signal and waiting for a response");
//             uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
//             arm_payload arm = { false };
//             const uint8_t message_length = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t*>(&arm), sizeof(arm), message, sizeof(message));
//             if (message_length == 0)
//             {
//                 Serial.println("ERROR: Failed to encode arm signal. Try again?");
//                 return;
//             }
//             radio.send(message, message_length);
//         }
//         else
//         {
//             Serial.println("Invalid command entered. Try again");
//             return;
//         }
//         
//         bool got_signal = false;
//         for (int i = 0; i < 3; i++)
//         {
//             if (radio.message_available())
//             {
//                 uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
//                 uint8_t message_length = sizeof(message);
//                 if (radio.receive(message, message_length))
//                 {
//                     protocol_t::packet packet = {};
//                     const bool decode_success = protocol_t::decode(message, message_length, packet);
//                     if (decode_success && packet.type == ARM_PAYLOAD_TYPE)
//                     {
//                         Serial.println("Received " + String(reinterpret_cast<arm_payload*>(packet.payload)->arm ? "arm" : "disarm") + " signal from flight computer");
//                         got_signal = true;
//                         break;
//                     }
//                 }
//             }
//             delay(1000);
//         }
//         if (!got_signal)
//         {
//             Serial.println("No signal received from flight computer within time. Assume it was not armed/disarmed");
//         }
//     }
// }

#include <Arduino.h>
#include "../../lib/sd/sd_card.h"

sd_t sd;

void setup()
{
    Serial.begin(115200);
    while (!Serial) { delay(1); }

    Serial.println("Initializing SD card...");
    if (!sd.init())
    {
        Serial.println("ERROR: SD card failed to initialize");
        while (true)
        {
        }
    }
    Serial.println("SD card initialized");
}

void loop()
{
    const bool ret = sd.write("Hello, data!");
    Serial.println(ret ? "Data written successfully" : "Data write failed");
    delay(1000);
}
