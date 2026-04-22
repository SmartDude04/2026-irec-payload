#include "radio.h"

// Definitions for specific pinouts
#define RFM95_CS 4
#define RFM95_INT 3
#define RFM95_RST 2

// Frequency definition
#define RF95_FREQ 433.0

radio_t::radio_t() : rf95(RFM95_CS, RFM95_INT)
{
    
}

bool radio_t::init()
{
    // Set RST and buitin LED as outputs
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RFM95_RST, OUTPUT); // Pulled to HIGH by default, which is reset. Pull to LOW for normal operation
    digitalWrite(RFM95_RST, HIGH);
    
    // Peform a manual reset of the radio by toggling the reset pin
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    
    // Attempt to initialize the radio 5 times before failing
    bool init_success = false;
    for (int i = 0; i < 5; i++)
    {
        if (rf95.init())
        {
            init_success = true;
            break;
        }
    }
    
    if (!init_success)
    {
        return false;
    }
    
    // Attempt to set the frequency of the radio
    if (!rf95.setFrequency(RF95_FREQ))
    {
        return false;
    }
    
    // TODO: Do research into transmitter power as well as PA_BOOST and PA_DAC (found inside setTxPower documentation)
    // Set Transmitter power
    rf95.setTxPower(20, false); // Used to be 23 but that's not allowed per the documentation?
    
    return true;
}


bool radio_t::send(const String &message)
{
    // Make sure message is short enough
    if (message.length() > RH_RF95_MAX_MESSAGE_LEN)
    {
        return false;
    }
    
    // Convert the string into a char[]
    char radiopacket[message.length() + 1];
    message.toCharArray(radiopacket, sizeof(radiopacket));
    const bool send_success = rf95.send(reinterpret_cast<uint8_t*>(radiopacket), message.length());
    if (!send_success)
    {
        return false;
    }
    
    // Wait until the packet is fully sent
    rf95.waitPacketSent();
    
    return true;
}

bool radio_t::send(const uint8_t *const message, const uint8_t message_length)
{
    if (message_length > RH_RF95_MAX_MESSAGE_LEN)
    {
        return false;
    }
    
    // Attempt to send the packet
    const bool send_success = rf95.send(message, message_length);
    if (!send_success)
    {
        return false;
    }
    
    // Wait until the packet is fully sent
    rf95.waitPacketSent();
    
    return true;
}

bool radio_t::wait_until_message(const int timeout)
{
    return rf95.waitAvailableTimeout(timeout);
}

bool radio_t::message_available()
{
    return rf95.available();
}

bool radio_t::receive(String &message)
{
    if (rf95.available())
    {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN + 1];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len))
        {
            buf[len] = '\0';
            message = reinterpret_cast<char*>(buf);
            return true;
        }
    }
    return false;
}

bool radio_t::receive(uint8_t *message, uint8_t &message_length)
{
    if (rf95.available())
    {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (rf95.recv(buf, &len))
        {
            if (len <= message_length)
            {
                memcpy(message, buf, len);
                message_length = len;
                return true;
            }
        }
    }
    return false;
}

int16_t radio_t::last_rssi()
{
    return rf95.lastRssi();
}
