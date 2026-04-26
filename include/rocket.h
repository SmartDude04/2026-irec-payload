#ifndef INC_2026_IREC_PAYLOAD_ROCKET_H
#define INC_2026_IREC_PAYLOAD_ROCKET_H

#include <accelerometer.h>
#include <altimeter.h>
#include <sd_card.h>
#include <radio.h>
#include <pressure.h>
#include <scd.h>
#include <protocol.h>
#include <Servo.h>

constexpr bool DEBUG = false;
constexpr double EXPERIMENT_ACCEL_THRESHOLD = DEBUG ? 6.0 : 3.0;
constexpr double EXPERIMENT_ALTITUDE_THRESHOLD = DEBUG ? 0.0 : 150.0;
constexpr double ACCELERATION_GROUND = 9.8;
constexpr unsigned long RADIO_WAIT_TIME = 100;

constexpr bool RADIO = false;
constexpr bool PRESSURE = true;
constexpr bool SCD = true;
constexpr bool SERVO = true;
constexpr bool ALTIMETER = true;
constexpr bool ACCELEROMETER = true;
constexpr bool SD_CARD = true;

extern radio_t radio;
extern pressure_t pressure;
extern scd_t scd;
extern Servo servo;
extern protocol_t protocol;
extern accelerometer_t accelerometer;
extern altimeter_t altimeter;
extern sd_t sd;

extern double acceleration_norm;
extern double altitude_m;
extern bool experiment_started;
extern bool armed;
extern uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];

// TODO: Add documentation
void update_acceleration_altitude();
void handle_incoming_radio_messages();
bool send_arm_ack(bool arm_state);
void send_environmental_packet();
void update_experiment_state();
bool send_data(const uint8_t *data, uint8_t len, const char *str_data);
void recover_i2c_bus();

#endif
