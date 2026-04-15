#include "pressure.h"

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define HPA_TO_PSI 0.01450377377

pressure::pressure() = default;

bool pressure::init()
{
    // Try to start the sensor 5 times
    for (int i = 0; i < 5; i++)
    {
        if (mprls.begin())
        {
            return true;
        }
        delay(100);
    }
    return false;
}

float pressure::get_pressure_hpa()
{
    return mprls.readPressure();
}

float pressure::get_pressure_psi()
{
    return mprls.readPressure() * HPA_TO_PSI;
}