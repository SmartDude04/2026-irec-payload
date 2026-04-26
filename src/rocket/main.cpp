#include <rocket.h>
#include <payloads/string_payload.h>
#include <Wire.h>

radio_t radio;
pressure_t pressure;
scd_t scd;
Servo servo;
protocol_t protocol;
accelerometer_t accelerometer;
altimeter_t altimeter;
sd_t sd;

double acceleration_norm = -1;
double altitude_m = 0;
bool experiment_started = false;
bool armed = true;
uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];

void setup() // TODO: Make into functions
{
    // Begin serial output
    Serial.begin(115200);
    if constexpr (DEBUG) { while (!Serial) { delay(1); } }
    
    // Start I2C and wait 2 seconds to ensure all sensors are booted up
    Wire.begin();
    delay(2000);
    
    const bool radio_init = RADIO ? radio.init() : false;
    const bool pressure_init = PRESSURE ? pressure.init() : false;
    const bool scd_init = SCD ? scd.init() : false;
    const bool servo_init = SERVO ? servo.attach(5) != INVALID_SERVO : false;
    if (servo_init) servo.write(0);
    const bool altimeter_init = ALTIMETER ? altimeter.init() : false;
    const bool accelerometer_init = ACCELEROMETER ? accelerometer.init() : false;
    const bool sd_init = SD_CARD ? sd.init() : false;
    
    if constexpr (DEBUG)
    {
        if constexpr (RADIO)
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
        
        if constexpr (PRESSURE)
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
        
        if constexpr (SCD)
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
        
        if constexpr (SERVO)
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
        
        if constexpr (ALTIMETER)
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
        
        if constexpr (ACCELEROMETER)
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
        
        if constexpr (SD_CARD)
        {
            if (!sd_init)
            {
                Serial.println("ERROR: SD card failed to initialize");
            }
            else
            {
                Serial.println("SD card initialized");
            }
        }
    }
    
    if ((RADIO && !radio_init) || (PRESSURE && !pressure_init) || (SCD && !scd_init) || (SERVO && !servo_init) || (ALTIMETER && !altimeter_init) || (ACCELEROMETER && !accelerometer_init) || (SD_CARD && !sd_init))
    {
        // If the radio fine but others are bad, send a message over the radio. Otherwise, there's not much we can do
        if (RADIO && radio_init)
        {
            String message = "ERROR: Failed to initialize the following sensors: ";
            message += PRESSURE && !pressure_init ? " Pressure " : "";
            message += SCD && !scd_init ? " SCD30 " : "";
            message += SERVO && !servo_init ? " Servo " : "";

            string_payload fatal;
            strcpy(fatal.message, message.c_str());
            
            uint8_t ret = protocol.encode(STRING_PAYLOAD_TYPE, reinterpret_cast<uint8_t *>(&fatal), sizeof(fatal), radio_packet, sizeof(radio_packet));
            if (ret == 0)
            {
                if constexpr (DEBUG)
                {
                    Serial.println("Failed to encode the string. Can't send over the radio.");
                }
            }
            else
            {
                ret = radio.send(radio_packet, ret);
                if (!ret)
                {
                    if constexpr (DEBUG)
                    {
                        Serial.println("Failed to send the packet over the radio.");
                    }
                }
            }
        }
        if constexpr (DEBUG)
        {
            Serial.println("ERROR: Failed to initialize sensor(s)... Program execution halted");
        }
        while (true)
        {
        }
    }
    
    if constexpr (!DEBUG)
    {
        servo.write(50);
        delay(2000);
        servo.write(0);
    }
}

void loop()
{
    // handle_incoming_radio_messages();
    send_environmental_packet();
    update_acceleration_altitude();
    update_experiment_state();
    
    delay(100);
}