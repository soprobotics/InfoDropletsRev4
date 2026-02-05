#include "Log.h"
#include "RadioFunctions/RadioFunctions.h"

namespace Log {
    void Log(const String& message) {
        RadioFunctions::SendMessage("[LOG] " + message);
    }

    void Error(const String& message) {
        RadioFunctions::SendMessage("[ERROR] " + message);
    }

    void Success(const String& message) {
        RadioFunctions::SendMessage("[SUCCESS] " + message);
    }
}