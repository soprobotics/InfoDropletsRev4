#include "Utils.h"

namespace Utils
{
    void SetupSSPins()
    {
        pinMode(DropletData::ss_R, OUTPUT);
        pinMode(DropletData::ss_SD, OUTPUT);
    }

    void SelectSD()
    {
        digitalWrite(DropletData::ss_R, HIGH);
        digitalWrite(DropletData::ss_SD, LOW); //active low
    }
    void SelectRadio()
    {
        digitalWrite(DropletData::ss_SD, HIGH);
        digitalWrite(DropletData::ss_R, LOW); //active low
    }
}
