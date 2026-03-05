#ifndef INC_2026_IREC_PAYLOAD_RADIO_H
#define INC_2026_IREC_PAYLOAD_RADIO_H

#include <WString.h>
#include <RH_RF95.h>

class radio
{
public:
    radio();

    /**
     * Initializes the radio set to the following pinouts:
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
     * Transmits the given raw bytes with the radio and waits until transmission is complete before returning
     * @param message Raw message to send
     * @param message_length Length of the message packet
    * @return True if successfully sent, false otherwise. This could be due to the message length being too long
     * (check RH_RF95_MAX_MESSAGE_LEN) or if the radio failed to send the message.
     */
    bool send(const uint8_t *message, size_t message_length);

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
     * Attempts to receive a message from the radio and convert it into a string
     * @param message String where the message will be stored
     * @return True if successfully stored the message; false otherwise
     */
    bool receive(String &message);

    /**
     * Attempts to receive a message from the radio and convert it into raw bytes
     * @param message Location where the message will be stored
     * @param message_length The maximum possible message length for this buffer should be passed in, and on success the
     * value will change to the size of the message received
     * @return True if successfully stored the message; false otherwise. This could be due to a radio failure or too
     * small a buffer. It is always recommended to make a buffer of size RH_RF95_MAX_MESSAGE_LEN + 1
     */
    bool receive(uint8_t *message, size_t &message_length);

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