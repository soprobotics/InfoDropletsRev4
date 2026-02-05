#include "RadioFunctions.h"
#include "../Log.h"

namespace RadioFunctions
{
    void Setup(const int& ss, const int& reset, const long& frequency){
        pinMode(ss, OUTPUT);
        pinMode(reset, OUTPUT);

        LoRa.setPins(ss, reset);
        if(!LoRa.begin(frequency))
        {
            while(true)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(200);
                digitalWrite(LED_BUILTIN, LOW);
                delay(200);
                digitalWrite(LED_BUILTIN, HIGH);
                delay(500);
                digitalWrite(LED_BUILTIN, LOW);
                delay(500);
            }
        }
        Log::Success("LoRa started");
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
}