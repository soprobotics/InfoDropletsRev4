#pragma once

#include <Arduino.h>
#include <SD.h>

namespace SdFunctions {
    void Setup(const int& ss); //Init SD card

    void WriteFile(const String& data, const String& filename); //Write data to file
}