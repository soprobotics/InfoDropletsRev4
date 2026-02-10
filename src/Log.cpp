#include "Log.h"
#include "RadioFunctions/RadioFunctions.h"

namespace Log {
    bool isDebugEnabled = true;

    void Log(const String& message) {
        RadioFunctions::SendMessage("[LOG] " + message);
    }

    void Debug(const String& message) {
        if(isDebugEnabled){
            RadioFunctions::SendMessage("[DEBUG] " + message);
        }
    }

    void Error(const String& message) {
        RadioFunctions::SendMessage("[ERROR] " + message);
    }

    void Success(const String& message) {
        RadioFunctions::SendMessage("[SUCCESS] " + message);
    }
}