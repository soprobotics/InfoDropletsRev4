#pragma once

#include <Arduino.h>

struct DropletData{
    const static int ss_R = 2;
    const static int ss_SD = 3;
};

namespace Utils{
    void SetupSSPins();
    void SelectSD();
    void SelectRadio();
}