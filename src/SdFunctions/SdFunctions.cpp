#include "SdFunctions.h"
#include "../Log.h"

namespace SdFunctions {

    bool Setup(const int& ss){
        if(!SD.begin(ss)){
            Log::Error("SD initialization failed.");
            return false;
        }

        Log::Success("SD initialization done.");
        return true;
    }

    void WriteFile(const String& filename, const String& content){
        File myFile = SD.open(filename, FILE_WRITE);
        if(!myFile){
            Log::Error("Could not open file: %s", filename);
        } else {
            myFile.print(content);
            myFile.close();
            Log::Success("File write: %s", filename);
        }
    }

}