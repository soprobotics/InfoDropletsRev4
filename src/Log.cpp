#include <stdarg.h>
#include "Log.h"
#include "RadioFunctions/RadioFunctions.h"

constexpr size_t LOG_BUFFER_SIZE = 128;

namespace Log {
    bool isDebugEnabled = true;

    void sendWithArgs(const char* prefix, const char* format, va_list args)
    {
        static char buffer[LOG_BUFFER_SIZE];

        int prefixLen = snprintf(buffer, sizeof(buffer), "%s", prefix);

        if (prefixLen < 0 || prefixLen >= LOG_BUFFER_SIZE)
            return;

        vsnprintf(
            buffer + prefixLen, //points to the position after the prefix
            sizeof(buffer) - prefixLen,
            format,
            args
        );

        RadioFunctions::SendMessage(buffer);
    }

    void Log(const char* format, ...) {
        va_list args;
        va_start(args, format);           
        sendWithArgs("[LOG] ", format, args);
        va_end(args);
    }

    void Debug(const char* format, ...) {
        if(isDebugEnabled){
            va_list args;
            va_start(args, format);
            sendWithArgs("[DEBUG] ", format, args);
            va_end(args);
        }
    }

    void Error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        sendWithArgs("[ERROR] ", format, args);
        va_end(args);
    }

    void Success(const char* format, ...) {
        va_list args;
        va_start(args, format);
        sendWithArgs("[SUCCESS] ", format, args);
        va_end(args);
    }

    //todo add logBase instead
}