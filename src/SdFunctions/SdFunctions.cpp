#include "SdFunctions.h"
#include "../Utils/Utils.h"
#include "../Log/Log.h"

namespace SdFunctions {
    constexpr size_t MAX_FILENAME_LENGTH = 20;
    constexpr uint8_t FILENAME_OFFSET = 5;
    char workingDir[MAX_FILENAME_LENGTH];

    File openFileInWorkingDir(const char* filename, uint8_t mode)
    {
        // filename starts position
        char* filenameStart = &workingDir[FILENAME_OFFSET];

        // save the original end position
        char backup = *filenameStart;

        // Append filename into the existing buffer
        while (*filename)
        {
            *filenameStart++ = *filename++;
        }

        *filenameStart = '\0';

        File file = SD.open(workingDir, mode);

        // Restore working directory
        *filenameStart = backup;

        return file;
    }

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
        bool tryFileWrite = WriteFile(testFileName, testFileContent, false);
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

    byte createWorkingDir(){
        byte counter = 0;
        strcpy(workingDir, "/v01/");
        do{
            counter++;
            if(counter < 10){
                workingDir[3] = counter + '0';
            }
            else{
                workingDir[2] = (counter / 10) + '0';
                workingDir[3] = (counter % 10) + '0';
            }
        }while(SD.exists(workingDir));

        if(!SD.mkdir(workingDir)){
            return 0;
        }

        return counter;
    }
    
    byte Setup(const int ss){
        Utils::SelectSD();

        if(!SD.begin(ss)){
            Log::Error(F("SD initialization failed!"));
            return 0;
        }

        if(!validateCardFunctions()){
            Log::Error(F("SD card test failed!"));
            return 0;
        }

        byte workingDirIteration = createWorkingDir();
        if(workingDirIteration == 0){
            Log::Error(F("Could not create working directory!"));
            return 0;
        }
        
        return workingDirIteration;
    }

    bool WriteFile(const char* filename, const char* content, bool useWorkingDir){
        Utils::SelectSD();

        File file;
        if(useWorkingDir){
            file = openFileInWorkingDir(filename, FILE_WRITE);
        }
        else{
            file = SD.open(filename, FILE_WRITE);
        }
        if(file){
            file.println(content);
            file.close();
            return true;
        }
        else{
            Serial.println("Could not open file for writing");
            return false;
        }
    }

    bool WriteStartupData(const char *fileName, const StartupData &startupData)
    {
        Utils::SelectSD();

        File file = openFileInWorkingDir(fileName, FILE_WRITE);
        if(file){
            file.print(F("Radio Setup OK: "));
            file.println(startupData.radioSetupOk ? "Yes" : "No");
            file.print(F("SD Setup OK: "));
            file.println(startupData.sdSetupOk ? "Yes" : "No");
            file.print(F("GPS Setup OK: "));
            file.println(startupData.gpsSetupOk ? "Yes" : "No");
            file.print(F("GPS High Altitude Mode OK: "));
            file.println(startupData.gpsHighAltOk ? "Yes" : "No");
            file.print(F("File Version: "));
            file.println(startupData.Version);
            file.close();
            return true;
        }
        else{
            Log::Error(F("Could not open startup data file for writing"));
        }

        return false;
    }
}