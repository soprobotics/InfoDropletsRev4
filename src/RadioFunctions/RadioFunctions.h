#pragma once

#include <Arduino.h>
#include <LoRa.h>

namespace RadioFunctions
{
    bool Setup(const int& ss, const int& reset, const long& frequency);

    String ReadMessage();

    void SendMessage(const __FlashStringHelper* message);
    //void SendMessageRuntime(const char* message);
    void SendPrefixedMessage(const __FlashStringHelper* prefix, const __FlashStringHelper* message);
    void SendPrefixedMessageRuntime(const __FlashStringHelper* prefix, const char* message);

    void End();
}