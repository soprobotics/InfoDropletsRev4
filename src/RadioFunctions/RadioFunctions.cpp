#include "RadioFunctions.h"
#include "../Utils/Utils.h"

namespace RadioFunctions
{
    bool Setup(const int& ss, const int& reset, const long& frequency){
        pinMode(ss, OUTPUT);
        pinMode(reset, OUTPUT);

        LoRa.setPins(ss, reset);
        if(!LoRa.begin(frequency))
        {
            Serial.println(F("LoRa init failed."));
            return false;
        }
        Serial.println(F("LoRa started"));

        SendMessage(F("LoRa started"));
        return true;
    }

    String ReadMessage(){
        String message = "";

        int packetSize = LoRa.parsePacket();
        while(!packetSize)
        {
            packetSize = LoRa.parsePacket();
        }

        while (LoRa.available()) {
            message += (char)LoRa.read();
        }

        return message;

    }

    void SendMessage(const __FlashStringHelper* message){
        Utils::SelectRadio();

        LoRa.beginPacket();
        LoRa.print(message);
        LoRa.endPacket();
        
        Utils::SelectSD();
    }

    void SendPrefixedMessage(const __FlashStringHelper* prefix, const __FlashStringHelper* message)
    {
        Utils::SelectRadio();

        LoRa.beginPacket();

        LoRa.print(prefix);
        LoRa.print(message);

        LoRa.endPacket();

        Utils::SelectSD();
    }

    void SendPrefixedMessageRuntime(const __FlashStringHelper* prefix, const char* message)
    {
        Utils::SelectRadio();

        LoRa.beginPacket();

        LoRa.print(prefix);
        LoRa.print(message);

        LoRa.endPacket();

        Utils::SelectSD();
    }

    void End(){
        LoRa.end();
    }
}