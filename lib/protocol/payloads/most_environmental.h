#ifndef INC_2026_IREC_PAYLOAD_MOST_ENVIRONMENTAL_H
#define INC_2026_IREC_PAYLOAD_MOST_ENVIRONMENTAL_H

#define MOST_ENVIRONMENTAL_TYPE 4

typedef struct
{
    float pressure_hpa;
    float altitude_m;
    float acceleration_g;
} most_environmental_payload;

#endif
