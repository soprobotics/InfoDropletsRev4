#pragma once

#include <Arduino.h>
#include <SD.h>

namespace SdFunctions {
    bool Setup(const int& ss); //Init SD card

    void WriteFile(const String& data, const String& filename); //Write data to file
}