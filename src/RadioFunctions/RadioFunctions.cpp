#include "RadioFunctions.h"
#include "../Log.h"

namespace RadioFunctions
{
    bool Setup(const int& ss, const int& reset, const long& frequency){
        pinMode(ss, OUTPUT);
        pinMode(reset, OUTPUT);

        LoRa.setPins(ss, reset);
        if(!LoRa.begin(frequency))
        {
            Log::Error("LoRa init failed.");
            return false;
        }
        Log::Success("LoRa started");
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

    void SendMessage(const String& message){
        LoRa.beginPacket();
        LoRa.print(message);
        LoRa.endPacket();
    }

    void End(){
        LoRa.end();
    }
}