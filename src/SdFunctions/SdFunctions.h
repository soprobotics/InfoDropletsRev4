#pragma once

#include <Arduino.h>
#include <SD.h>

namespace SdFunctions {
    bool Setup(const int ss); //Init SD card

    bool WriteFile(const char* filename, const char* content); //Write data to file
}