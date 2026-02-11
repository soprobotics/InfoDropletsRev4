#include <Arduino.h>
#include <EEPROM.h>

#include "SdFunctions/SdFunctions.h"
#include "RadioFunctions/RadioFunctions.h"
#include "GpsFunctions/GpsFunctions.h"
#include "Log.h"

#define EEPROM_ID_ADDR 0

byte deviceId;

void setup() {

  bool RadioSetupOk = RadioFunctions::Setup(2,5,868E6);
  bool SdInitOk = SdFunctions::Setup(3);
  bool GpsInitOk = GpsFunctions::Setup();
  
  if(RadioSetupOk && (!SdInitOk || !GpsInitOk)) {
    Log::Error("Fatal Error during setup");
    RadioFunctions::End();
    SPI.end();
    pinMode(13, OUTPUT);
    while(true){    
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
      delay(200);
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
      delay(200);
      digitalWrite(13, HIGH);
      delay(600);
      digitalWrite(13, LOW);
      delay(600);
    }
  }

  GpsFunctions::SetNavMode(); //Proceed if navmode setup fails

  Log::Success("__________ Setup complete __________");
  delay(1000);
}

void loop() {
  RadioFunctions::SendMessage("\t\t\t\t\t\t\t\t\t" + GpsFunctions::Measure(333,6));

  delay(50);
}