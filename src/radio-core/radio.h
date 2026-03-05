#ifndef INC_2026_IREC_PAYLOAD_RADIO_H
#define INC_2026_IREC_PAYLOAD_RADIO_H
#include <WString.h>

#include "RH_RF95.h"

class radio
{
public:
    explicit radio();

    /**
     * Initalizes the radio set to the following pinouts:
     * - 5V  -> VIN
     * - GND -> GND
     * - 3   -> G0
     * - 13  -> SCK
     * - 12  -> MISO
     * - 11  -> MOSI
     * - 4   -> CS
     * - 2   -> RST
     * @return True if init() succeeded, false otherwise
     */
    bool init();

    /**
     * Transmits the given message with the radio and waits until transmission is complete before returning
     * @param message Message to send
     * @return True if successfully sent, false otherwise. This could be due to the message length being too long
     * (check RH_RF95_MAX_MESSAGE_LEN) or if the radio failed to send the message.
     */
    bool send(const String& message);

    /**
     * Blocks for the specified number of milliseconds for a message from another radio to be received. If no message
     * is received before the timeout, the function returns and the return value is false.
     * @param timeout Timeout, in milliseconds, to wait for a message to be received
     * @return True if a message has been received, false if the timeout has passed and a message has not been received
     */
    bool wait_until_message(int timeout);

    /**
     * Returns whether a new message is available on the radio
     * @return True if a message has been received; false otherwise
     */
    bool message_available();

    /**
     * If a message is ready to be received, it returns that message. Otherwise, a blank string is returned
     * @return Message receieved
     */
    String receive();

    /**
    * Returns the most recent RSSI (Receiver Signal Strength Indicator). Usually it is the RSSI of the last received
    * message, which is measured when the preamble is received.
    * @return The most recent RSSI measurement in dBm.
     */
    int16_t last_rssi();
private:
    RH_RF95 rf95;
};


#endif