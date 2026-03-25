#ifndef INC_2026_IREC_PAYLOAD_STRING_PAYLOAD_H
#define INC_2026_IREC_PAYLOAD_STRING_PAYLOAD_H

#define STRING_PAYLOAD_TYPE 0

typedef struct
{
    char message[128];
} string_payload;

#endif