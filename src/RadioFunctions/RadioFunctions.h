#pragma once

#include <Arduino.h>
#include <LoRa.h>

namespace RadioFunctions
{
    void Setup(const int& ss, const int& reset, const long& frequency);

    String ReadMessage();

    void SendMessage(const String& message);
}