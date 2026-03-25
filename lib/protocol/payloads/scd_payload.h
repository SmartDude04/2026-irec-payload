#ifndef INC_2026_IREC_PAYLOAD_SCD_DATA_H
#define INC_2026_IREC_PAYLOAD_SCD_DATA_H

#define SCD_PAYLOAD_TYPE 2

typedef struct
{
    float temperature;
    float humidity;
    float CO2;
} scd_payload;


#endif