#pragma once

#include <Arduino.h>

namespace Log {
    extern bool isDebugEnabled;

    void Log(const __FlashStringHelper* message);
    void LogRuntime(const char* message);

    void Debug(const __FlashStringHelper* message);
    void DebugRuntime(const char* message);

    void Error(const __FlashStringHelper* message);
    void ErrorRuntime(const char* message);
}