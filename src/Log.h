#pragma once

#include <Arduino.h>

namespace Log {
    extern bool isDebugEnabled;

    void Log(const String& message);

    void Debug(const String& message);

    void Error(const String& message);

    void Success(const String& message);
}