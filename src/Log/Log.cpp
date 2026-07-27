#include "Log.h"
#include "RadioFunctions/RadioFunctions.h"

namespace Log {
    bool isDebugEnabled = true;
    void Debug(const __FlashStringHelper* message){
        if(isDebugEnabled){
            RadioFunctions::SendPrefixedMessage(F("DBUG: "), message);
        }
    }
    void DebugRuntime(const char* message){
        if(isDebugEnabled){
            RadioFunctions::SendPrefixedMessageRuntime(F("DBUG: "), message);
        }
    }

    void Log(const __FlashStringHelper* message){
        RadioFunctions::SendPrefixedMessage(F("LOG: "), message);
    }
    void LogRuntime(const char* message){
        RadioFunctions::SendPrefixedMessageRuntime(F("LOG: "), message);
    }

    void Error(const __FlashStringHelper* message){
        RadioFunctions::SendPrefixedMessage(F("ERROR: "), message);
    }
    void ErrorRuntime(const char* message){
        RadioFunctions::SendPrefixedMessageRuntime(F("ERROR: "), message);
    }
}