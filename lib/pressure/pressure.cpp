#include "pressure.h"

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  8  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    7  // set to any GPIO pin to read end-of-conversion by pin
#define HPA_TO_PSI 0.01450377377

pressure::pressure() : mprls(RESET_PIN, EOC_PIN)
{
}

bool pressure::init()
{
    const bool ret = mprls.begin();
    if (!ret)
    {
        return false;
    }
    
    pinMode(RESET_PIN, OUTPUT);
    digitalWrite(RESET_PIN, HIGH);
    pinMode(EOC_PIN, INPUT);
    return true;
}

void pressure::reset()
{
    digitalWrite(RESET_PIN, LOW);
    delay(100);
    digitalWrite(RESET_PIN, HIGH);
}

float pressure::get_pressure_hpa()
{
    return mprls.readPressure();
}

float pressure::get_pressure_psi()
{
    return mprls.readPressure() * HPA_TO_PSI;
}