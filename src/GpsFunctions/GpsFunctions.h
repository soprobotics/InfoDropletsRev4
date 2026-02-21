#pragma once

#include <Arduino.h>
#include <TinyGPS++.h>
#include "../Log.h"

struct GpsData {
    double lat = 0;
    double lng = 0;
    double alt = 0;

    uint32_t sats = 0;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;

    bool hasTime = false;
    unsigned long timeMsElapsed = 0;

    bool hasFix = false;
    unsigned long fixMsElapsed = 0;

    unsigned long LastUpdateTime = 0;
};

namespace GpsFunctions {

    extern GpsData gpsData;
    extern TinyGPSPlus gps;

    bool Setup(const int& timeoutMs = 15000, const int& baudRate = 9600);
    bool SetNavMode();
    
    String ReadFromModule(const long& waitTimeMs);
    String Measure(const long& measureTime, const byte& dropletId = 99);
}