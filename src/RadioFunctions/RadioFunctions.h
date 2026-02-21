#pragma once

#include <Arduino.h>
#include <LoRa.h>

namespace RadioFunctions
{
    bool Setup(const int& ss, const int& reset, const long& frequency);

    String ReadMessage();

    void SendMessage(const char* message);

    void End();
}