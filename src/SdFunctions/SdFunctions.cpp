#include "SdFunctions.h"
#include "../Log.h"

namespace SdFunctions {

    void Setup(const int& ss){
        if(!SD.begin(ss)){
            Log::Error("SD initialization failed.");
        } else{
            Log::Success("SD initialization done.");
        }
    }

    void WriteFile(const String& filename, const String& content){
        File myFile = SD.open(filename, FILE_WRITE);
        if(!myFile){
            String errorMessage = "Could not open file: " + filename;
            Log::Error(errorMessage);
        } else {
            myFile.print(content);
            myFile.close();
            Log::Success("File write: " + filename);
        }
    }

}