#include <Arduino.h>
#include <EEPROM.h>

#include "SdFunctions/SdFunctions.h"
#include "RadioFunctions/RadioFunctions.h"
#include "GpsFunctions/GpsFunctions.h"
#include "Log/Log.h"
#include "Utils/Utils.h"


int deviceId = 99;

void reset() { asm volatile ("jmp 0x7800"); }

void setup() {

  bool RadioSetupOk = RadioFunctions::Setup(DropletData::ss_R, 5,868E6);
  deviceId = EEPROM.read(DropletData::DROPLET_ID_EEPROM_ADDR);
  

  Log::DebugRuntime(dropletIdMsg);

  bool SdInitOk = SdFunctions::Setup(DropletData::ss_SD);
  Log::Debug(F("SD setup done"));
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
  char datarow[49]; //longest possible measurement: 90;10;17:28:40;-48.268917;-116.403818;-17000.01 --> 47 chars + 1 safety + 1 null-terminator = 49
  GpsFunctions::Measure(datarow, sizeof(datarow), 1000,deviceId);
  Log::Log("\t\t\t\t\t\t%s", datarow);

  delay(50);
}