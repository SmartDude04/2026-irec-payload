#include "sd_card.h"

constexpr int SD_CS = 4;
constexpr int SD_CD = 2;

sd_t::sd_t() = default;

bool sd_t::init()
{
    pinMode(SD_CS, INPUT); // This sets the pin we aren't using to an input to avoid any weird stuff
    bool init_success = false;
    for (int i = 0; i < 5; i++)
    {
        init_success = SD.begin(SD_CS);
        if (init_success)
        {
            break;
        }
    }
    
    if (!init_success)
    {
        return false;
    }
    
    // Generate uuid for filename
    delay(10);
    randomSeed(analogRead(0) * analogRead(1));
    delay(10);
    do
    {
        const unsigned long uuid = random(0, static_cast<long>(pow(10,8)));
        filename = String(uuid) + ".txt";
    }
    while (SD.exists(filename.c_str()));
    return true;
}

bool sd_t::write(const char* data) const
{
    if (filename.length() == 0) return false;

    File sd_file = SD.open(filename.c_str(), FILE_WRITE);
    if (!sd_file) {
        Serial.println("SD.open failed");
        return false;
    }

    const bool write_success = sd_file.println(data);
    sd_file.close();
    return write_success;
}