#include "Utils.h"

namespace Utils
{
    void SetupSSPins()
    {
        pinMode(DropletData::R_ss, OUTPUT);
        pinMode(DropletData::SD_ss, OUTPUT);
    }

    void SelectSD()
    {
        digitalWrite(DropletData::R_ss, HIGH);
        digitalWrite(DropletData::SD_ss, LOW); //active low
    }
    void SelectRadio()
    {
        digitalWrite(DropletData::SD_ss, HIGH);
        digitalWrite(DropletData::R_ss, LOW); //active low
    }   
}
