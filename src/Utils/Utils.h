#pragma once

#include <Arduino.h>

struct DropletData{
    const static byte ss_R = 2;
    const static byte ss_SD = 3;
    const static int DROPLET_ID_EEPROM_ADDR = 0;
};

namespace Utils{
    void SetupSSPins();
    void SelectSD();
    void SelectRadio();
}