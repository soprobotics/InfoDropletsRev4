#include "GpsFunctions.h"

TinyGPSPlus gps;

namespace GpsFunctions
{
    String SmartDelay(const long& waitTimeMs){
        //Log::Log("Entered SmartDelay");
        String rawData = "";
        unsigned long start = millis();

        do
        {
            while (Serial.available()){
            char rawChar = Serial.read();
            gps.encode(rawChar); //!!!!!!
            rawData += rawChar;
            }
        } while (millis() - start < waitTimeMs);
        return rawData;
    }

    String Measure(bool beal, const byte& dropletId) {
        String _data;
        if (millis() > 500 && gps.charsProcessed() < 10) {
            Log::Error("No gps detected");
        }

        /* _data =  dropletId + ";" +
            String(Sat) + ";" +
            String(gps.time.hour()) + ":" +
            String(gps.time.minute()) + ":" +
            String(gps.time.second()) +  ";" +
            String(Lat, 6) + ";" +
            String(Long, 6) + ";" +
            String(Alt, 6); */

        char __data[50];

        snprintf(__data, sizeof(__data),
            "%u;%d;%02d:%02d:%02d;%.6f;%.6f;%.2f",
            dropletId,
            gps.satellites.value(),
            gps.time.hour(),
            gps.time.minute(),
            gps.time.second(),
            gps.location.lat(),
            gps.location.lng(),
            gps.altitude.meters());

        _data = __data;
        
        Log::Log("Data row constructed");  

        /* if (beal == 1) {
            beallt = 1;
        } else if (Lat != 0 && Sat != 0) {
            beallt = 0;
        } */
        return __data;
    }

    bool Setup(const int& timeoutMs, const int& baudRate) { //default wait time is 15 seconds
        Serial.begin(baudRate);
        while(!Serial);

        bool gpsDetected = false;
        unsigned long start = millis();
        while(millis() - start < timeoutMs && !gpsDetected){
            SmartDelay(100);
            if(gps.charsProcessed() > 10){
            gpsDetected = true;
            }
        }

        if(!gpsDetected){
            Log::Error("Gps not detected");
            return false;
        }

        Log::Success("GPS detected, setup done");
        return true;
    }
}
