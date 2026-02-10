#include "GpsFunctions.h"

namespace GpsFunctions
{
    TinyGPSPlus gps;
    GpsData gpsData;

    String SmartDelay(const long& waitTimeMs){
        String rawData = "";
        unsigned long start = millis();

        do
        {
            while (Serial.available()){
            char rawChar = Serial.read();
            gps.encode(rawChar);
            rawData += rawChar;
            }
        } while (millis() - start < waitTimeMs);
        return rawData;
    }

    void UpdateGpsData(){
        gpsData.sats = gps.satellites.value();
        gpsData.hours = gps.time.hour();
        gpsData.minutes = gps.time.minute();
        gpsData.seconds = gps.time.second();
        gpsData.lat = gps.location.lat();
        gpsData.lng = gps.location.lng();
        gpsData.alt = gps.altitude.meters();
    }

    String ConstructDataRow(const byte& dropletId = 99){
        char data[64];

        char latStr[16]; 
        char lngStr[16]; 
        char altStr[16]; 
        dtostrf(gpsData.lat, 0, 6, latStr); 
        dtostrf(gpsData.lng, 0, 6, lngStr); 
        dtostrf(gpsData.alt, 0, 2, altStr);

        snprintf(data, sizeof(data),
            "%u;%lu;%02u:%02u:%02u;%s;%s;%s",
            dropletId,
            gpsData.sats,
            gpsData.hours,
            gpsData.minutes,
            gpsData.seconds,
            latStr,
            lngStr,
            altStr);

        return data;
    }

    String Measure(const long& measureTime, const byte& dropletId) {

        SmartDelay(measureTime);
        UpdateGpsData();

        if(!gpsData.hasTime && gpsData.hours != 0 && gpsData.minutes != 0 && gpsData.seconds != 0){
            gpsData.hasTime = true;
            gpsData.timeMsElapsed = millis();
            Log::Log("GPS time acquired, took " + (String)gpsData.timeMsElapsed + " ms");
        }

        if(!gpsData.hasFix && gpsData.lat != 0 && gpsData.lng != 0 && gpsData.alt != 0){
            gpsData.hasFix = true;
            gpsData.fixMsElapsed = millis();
            Log::Log("GPS fix acquired, took " + (String)gpsData.fixMsElapsed + " ms");
        }
#
        return ConstructDataRow(dropletId); 
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
