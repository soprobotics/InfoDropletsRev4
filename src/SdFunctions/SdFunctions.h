#pragma once

#include <Arduino.h>
#include <SD.h>
#include "../Utils/Utils.h"

namespace SdFunctions {
    byte Setup(const int ss); //Init SD card

    bool WriteFile(const char* filename, const char* content, bool useWorkingDir = true); //Write data to file
    bool WriteStartupData(const char* fileName, const StartupData& startupData); //Write startup data to file
}