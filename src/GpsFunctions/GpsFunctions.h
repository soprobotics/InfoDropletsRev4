#pragma once

#include <Arduino.h>
#include <TinyGPS++.h>
#include "../Log.h"

namespace GpsFunctions {
    bool Setup(const int& timeoutMs = 15000, const int& baudRate = 9600); //Init GPS
    String SmartDelay(const long& waitTimeMs);
    String Measure(bool beal, const byte& dropletId = 99);
}