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

    void sendUBX(uint8_t *MSG, uint8_t len) {

        Serial.flush();
        Serial.write(0xFF);
        delay(100);

        for(int i=0; i<len; i++) {
            Serial.write(MSG[i]);
        }
    }

    bool getUBX_ACK(uint8_t *MSG) {
        uint8_t b;
        uint8_t ackByteID = 0;
        uint8_t ackPacket[10];
        unsigned long startTime = millis();

        //construct expected ACK packet
        ackPacket[0] = 0xB5;  // header
        ackPacket[1] = 0x62;  // header
        ackPacket[2] = 0x05;  // class
        ackPacket[3] = 0x01;  // id
        ackPacket[4] = 0x02;  // length
        ackPacket[5] = 0x00;
        ackPacket[6] = MSG[2];  // ACK class
        ackPacket[7] = MSG[3];  // ACK id
        ackPacket[8] = 0;   // CK_A
        ackPacket[9] = 0;   // CK_B

        for (uint8_t i=2; i<8; i++) {
            ackPacket[8] = ackPacket[8] + ackPacket[i];
            ackPacket[9] = ackPacket[9] + ackPacket[8];
        }
        
        while (1) {
            if (ackByteID > 9) {
                return true;
            }

            // Timeout if no valid response in 60 seconds
            if (millis() - startTime > 3000) {
                Log::Error("Set Navmode TIMEOUT, retrying...");
                return false;
            }

            // Make sure data is available to read
            if (Serial.available()) {
                b = Serial.read();

                // Check that bytes arrive in sequence as per expected ACK packet
                if (b == ackPacket[ackByteID]) {
                    ackByteID++;
                    Log::Debug(String(ackByteID)+". ACK byte match.");
                }
                else {
                    ackByteID = 0;  // Reset and look again, invalid order
                }
            }
        }
    }

    bool SetNavMode() {
        Log::Log("Setting Navmode...");
        
        uint8_t setNavMode6[] = {
            0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06,  //NavMode bit - 1 stationary, 5 sea 500m, 6 50km <1g
            0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
            0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C,
            0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC
        };
        
        int counter = 0;
        bool gps_set_sucess = false;
        while(!gps_set_sucess && counter < 10){
            sendUBX(setNavMode6, sizeof(setNavMode6)/sizeof(uint8_t));
            gps_set_sucess = getUBX_ACK(setNavMode6);
            counter++;
            delay(50);
        }
        delay(100);

        if(!gps_set_sucess){
            Log::Error("Failed to set and validate Navmode after 10 attempts");
            return false;
        }
        
    Log::Success("Navmode set and validated successfully");
        return true;
    }

    bool Setup(const int& timeoutMs, const int& baudRate) { //default wait time is 15 seconds
        Serial.begin(baudRate);
        while(!Serial);

        unsigned long start = millis();
        while(millis() - start < timeoutMs){
            SmartDelay(100);
            if(gps.charsProcessed() > 10){
                Log::Success("GPS detected");
                return true;
            }
        }

        Log::Error("Gps not detected");
        return false;
    }
}