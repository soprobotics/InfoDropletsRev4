#pragma once

#include <Arduino.h>

namespace Log {
    void Log(const String& message);

    void Error(const String& message);

    void Success(const String& message);
}