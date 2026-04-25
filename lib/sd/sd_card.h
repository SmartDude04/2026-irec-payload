#ifndef INC_2026_IREC_PAYLOAD_SD_H
#define INC_2026_IREC_PAYLOAD_SD_H

#include <SD.h>

class sd_t
{
public:
    sd_t();

    /**
     * Attempts to initialize the MicroSD card module and create a file with a unique name
     * @return True if successfully initialized and file created; false otherwise
     */
    bool init();

    /**
     * Attempts to write the string data to the file
     * @param data String to write to the file
     * @return True if successfully written; false otherwise
     */
    bool write(const char *data) const;
private:
    String filename;
};


#endif
