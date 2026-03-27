#include <Arduino.h>
#include <radio.h>
#include <pressure.h>
#include <scd.h>
#include <protocol.h>
#include <payloads/string_payload.h>
#include <payloads/arm_payload.h>
#include <payloads/most_environmental.h>
#include <payloads/all_environmental.h>
#include <Servo.h>
#include <Adafruit_BMP280.h>
#include <MPU6050.h>

#define EXPERIMENT_ACCEL_THRESHOLD 100

constexpr bool RADIO = true;
constexpr bool PRESSURE = false;
constexpr bool SCD = false;
constexpr bool SERVO = false;
constexpr bool ACCELEROMETER = true;
constexpr bool ALTIMETER = true;

radio radio;
pressure pressure;
scd scd;
MPU6050 mpu;
Servo servo;
protocol protocol;
Adafruit_BMP280 bmp;

uint8_t microgravity_counter = 0;
bool experiment_started = false;
bool armed = false;

void setup()
{
    // Begin serial output
    Serial.begin(115200);
    while (!Serial) { delay(1); }
    
    // Serial.println("Initializing...");
    
    const bool radio_init = RADIO ? radio.init() : false;
    // Serial.println("Radio initialized");
    const bool pressure_init = PRESSURE ? pressure.init() : false;
    // Serial.println("Pressure sensor initialized");
    const bool scd_init = SCD ? scd.init() : false;
    // Serial.println("SCD30 initialized");
    const bool servo_init = SERVO ? servo.attach(5) != INVALID_SERVO : false;
    // Serial.println("Servo initialized");
    mpu.initialize();
    delay(100);
    const bool accelerometer_init = ACCELEROMETER ? mpu.testConnection() : false;
    // Serial.println("Accelerometer initialized");
    const bool altimeter_init = ALTIMETER ? bmp.begin() : false;
    
    // Serial.println("Sensors initialized");
    
    // Initalize the radio
    if (RADIO)
    {
        if (!radio_init)
        {
            Serial.println("ERROR: Radio failed to initialize");
        }
        else
        {
            Serial.println("Radio initialized");
        }
    }
    
    if (PRESSURE)
    {
        if (!pressure_init)
        {
            Serial.println("ERROR: Pressure sensor failed to initialize");
        }
        else
        {
            Serial.println("Pressure sensor initialized");
        }
    }
    
    if (SCD)
    {
        if (!scd_init)
        {
            Serial.println("ERROR: SCD30 failed to initialize");
        }
        else
        {
            scd.set_measurement_interval(2);
            Serial.println("SCD30 initialized");
        }
    }
    
    if (SERVO)
    {
        if (!servo_init)
        {
            Serial.println("ERROR: Servo failed to initialize");
        }
        else
        {
            Serial.println("Servo initialized");
        }
    }
    
    if (ACCELEROMETER)
    {
        if (!accelerometer_init)
        {
            Serial.println("ERROR: Accelerometer failed to initialize");
        }
        else
        {
            Serial.println("Accelerometer initialized");
        }       
    }
    
    if (ALTIMETER)
    {
        if (!altimeter_init)
        {
            Serial.println("ERROR: Altimeter failed to initialize");
        }
        else
        {
            Serial.println("Altimeter initialized");
        }
    }
    
    if ((RADIO && !radio_init) || (PRESSURE && !pressure_init) || (SCD && !scd_init) || (SERVO && !servo_init) || (ACCELEROMETER && !accelerometer_init))
    {
        // If the radio fine but others are bad, send a message over the radio. Otherwise, there's not much we can do
        if (RADIO && radio_init)
        {
            String message = "ERROR: Failed to initialize the following sensors: ";
            message += PRESSURE && !pressure_init ? " Pressure " : "";
            message += SCD && !scd_init ? " SCD30 " : "";
            message += SERVO && !servo_init ? " Servo " : "";
            message += ACCELEROMETER && !accelerometer_init ? " Accelerometer " : "";
            message += ALTIMETER ? " Altimeter " : "";

            string_payload fatal;
            strcpy(fatal.message, message.c_str());
            
            uint8_t radio_packet[protocol::MIN_BYTES_FOR_PACKET];
            uint8_t ret = protocol.encode(STRING_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&fatal), sizeof(fatal), radio_packet, sizeof(radio_packet));
            if (ret == 0)
            {
                Serial.println("Failed to encode the string. Can't send over the radio.");
            }
            else
            {
                ret = radio.send(radio_packet, ret);
                if (!ret)
                {
                    Serial.println("Failed to send the packet over the radio.");
                }
            }
        }
        Serial.println("ERROR: Failed to initialize sensor(s)... Program execution halted");
        while (true)
        {
        }
    }
}

void loop()
{
    // If not armed, wait until the arm signal through the radio is given
    if (!armed)
    {
        while (1)
        {
            if (radio.message_available())
            {
                uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
                uint8_t message_length = sizeof(message);
                if (radio.receive(message, message_length))
                {
                    protocol::packet packet = {};
                    const bool decode_success = protocol::decode(message, message_length, packet);
                    if (decode_success && packet.type == ARM_PAYLOAD_TYPE && reinterpret_cast<arm_payload*>(packet.payload)->arm)
                    {
                        armed = true;
                        
                        // Send back a message confirming being armed
                        arm_payload arm_return;
                        arm_return.arm = true;
                        servo.write(0);
                        message_length = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&arm_return), sizeof(arm_return), message, sizeof(message));
                        radio.send(message, message_length);
                        break;
                    }
                }
            }
        }
    }
    
    // Check for any radio messages
    if (radio.message_available())
    {
        uint8_t message[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t message_length = sizeof(message);
        if (radio.receive(message, message_length))
        {
            protocol::packet packet = {};
            const bool decode_success = protocol::decode(message, message_length, packet);
            if (decode_success)
            {
                // Right now, the only meaningful type would be an unarm signal
                if (packet.type == ARM_PAYLOAD_TYPE)
                {
                    if (reinterpret_cast<arm_payload*>(packet.payload)->arm)
                    {
                        // We are already armed, but still send back a message confirming being armed
                        arm_payload arm_return;
                        arm_return.arm = true;
                        message_length = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&arm_return), sizeof(arm_return), message, sizeof(message));
                        radio.send(message, message_length);
                        return;
                    }
                    else
                    {
                        // Unarm and send that back to the ground station
                        arm_payload arm_return;
                        arm_return.arm = false;
                        experiment_started = false;
                        message_length = protocol.encode(ARM_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&arm_return), sizeof(arm_return), message, sizeof(message));
                        radio.send(message, message_length);
                        armed = false;
                        return;
                    }
                }
            }
        }
    }
    
    // Send environmental data
    if (scd::data_ready())
    {
        // Send an all_environmental payload
        all_environmental_payload payload;
        payload.pressure_hpa = pressure.get_pressure_hpa();
        payload.altitude_m = bmp.readAltitude();
        payload.acceleration_g = mpu.getAccelerationX();
        scd_data scd_data;
        scd.read_data(scd_data);
        payload.CO2 = scd_data.CO2;
        payload.temperature = scd_data.temperature;
        payload.humidity = scd_data.humidity;
        
        uint8_t radio_packet[protocol::MIN_BYTES_FOR_PACKET];
        const uint8_t ret = protocol.encode(ALL_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
        if (ret)
        {
            radio.send(radio_packet, ret);
        }
    }
    else
    {
        // Send a most_environmental payload
        most_environmental_payload payload;
        payload.pressure_hpa = pressure.get_pressure_hpa();
        payload.altitude_m = bmp.readAltitude();
        payload.acceleration_g = mpu.getAccelerationX();
        
        uint8_t radio_packet[protocol::MIN_BYTES_FOR_PACKET];
        const uint8_t ret = protocol.encode(MOST_ENVIRONMENTAL_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
        if (ret)
        {
            radio.send(radio_packet, ret);
        }
    }
    
    // If acceleration stays below the threshold for long enough, start the experiment!
    if (abs(mpu.getAccelerationX()) < EXPERIMENT_ACCEL_THRESHOLD && abs(mpu.getAccelerationY()) < EXPERIMENT_ACCEL_THRESHOLD && abs(mpu.getAccelerationZ()) < EXPERIMENT_ACCEL_THRESHOLD)
    {
        microgravity_counter++;
        if (microgravity_counter > 10)
        {
            // Starting the experiment!
            experiment_started = true;
            microgravity_counter = 0;
            
            // Send a message saying it has started
            string_payload payload;
            strcpy(payload.message, "Experiment started!");
            uint8_t radio_packet[protocol::MIN_BYTES_FOR_PACKET];
            const uint8_t ret = protocol.encode(STRING_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&payload), sizeof(payload), radio_packet, sizeof(radio_packet));
            if (ret)
            {
                radio.send(radio_packet, ret);
            }
            
            // Turn the servo
            servo.write(180);
        }
    }
    else
    {
        microgravity_counter = 0;
    }
    delay(100);
}