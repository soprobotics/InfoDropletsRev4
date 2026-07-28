#include "SdFunctions.h"
#include "../Utils/Utils.h"
#include "../Log/Log.h"

namespace SdFunctions {
    bool validateCardFunctions(){
        char testFileContent[] = "This is a test file for SD card validation, can be deleted.";
        char testFileName[] = "test.txt";

        //check for pre-existing test file and remove it if found
        if(SD.exists(testFileName)){
            if(!SD.remove(testFileName)){
                Log::Error(F("SD SETUP: Could not remove pre-existing test file"));
                return false;
            }
        }

        // create and write to a test file    
        bool tryFileWrite = WriteFile(testFileName, testFileContent);
        if(!tryFileWrite){
            Log::Error(F("SD SETUP: Failed to create test file"));
            return false;
        }

        //check if test file was created
        if(!SD.exists(testFileName)){
            Log::Error(F("SD SETUP: Test file was not found"));
            return false;
        }

        //validate test file content
        File testFile = SD.open(testFileName, FILE_READ);
        int i = 0;
        char buffer[2] = {'0', '\0'};
        while(testFile.available() && testFileContent[i] != '\0'){
            buffer[0] = testFile.read();

            if(buffer[0] != testFileContent[i]){
                testFile.close();
                Log::Error(F("SD SETUP: Test file content mismatch"));
                return false;
            }
            i++;
        }
        testFile.close();

        //remove test file
        if(!SD.remove(testFileName)){
            Log::Error(F("SD SETUP: Could not remove test file"));
            return false;
        }

        return true;
    }

    bool Setup(const int ss){
        Utils::SelectSD();

        if(!SD.begin(ss)){
            Log::Error(F("SD initialization failed!"));
            return false;
        }

        if(!validateCardFunctions()){
            Log::Error(F("SD card test failed!"));
            return false;
        }
        
        return true;
    }

    bool WriteFile(const char* filename, const char* content){
        Utils::SelectSD();

        File myFile = SD.open(filename, FILE_WRITE);
        if(myFile){
            myFile.println(content);
            myFile.close();
            return true;
        }
        else{
            Serial.println("Could not open file for writing");
            return false;
        }
    }
}