#ifndef INC_2026_IREC_PAYLOAD_ALL_ENVIRONMENTAL_H
#define INC_2026_IREC_PAYLOAD_ALL_ENVIRONMENTAL_H

#define ALL_ENVIRONMENTAL_TYPE 5

typedef struct
{
    float pressure_hpa;
    float altitude_m;
    float acceleration_g;
    float temperature;
    float humidity;
    float CO2;
} all_environmental_payload;

#endif