#ifndef INC_2026_IREC_PAYLOAD_ROCKET_H
#define INC_2026_IREC_PAYLOAD_ROCKET_H

#include "../../lib/accelerometer/accelerometer.h"
#include "../../lib/altimeter/altimeter.h"
#include <radio.h>
#include <pressure.h>
#include <scd.h>
#include <protocol.h>
#include <Servo.h>

constexpr bool DEBUG = true;
constexpr double EXPERIMENT_ACCEL_THRESHOLD = 0.1;
constexpr double ACCELERATION_GROUND = 9.8;
constexpr unsigned long RADIO_WAIT_TIME = 100;

extern radio_t radio;
extern pressure_t pressure;
extern scd_t scd;
extern Servo servo;
extern protocol_t protocol;
extern accelerometer_t accelerometer;
extern altimeter_t altimeter;

extern double acceleration_norm;
extern bool experiment_started;
extern bool armed;
extern uint8_t radio_packet[protocol_t::MIN_BYTES_FOR_PACKET];

// TODO: Add documentation
void avg_acceleration();
void handle_incoming_radio_messages();
bool send_arm_ack(bool arm_state);
void send_environmental_packet();
void update_experiment_state();

#endif
