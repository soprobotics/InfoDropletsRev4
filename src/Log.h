#pragma once

#include <Arduino.h>

namespace Log {
    extern bool isDebugEnabled;

    void Log(const char* message, ...);

    void Debug(const char* format, ...);

    void Error(const char* format, ...);

    void Success(const char* message, ...);
}