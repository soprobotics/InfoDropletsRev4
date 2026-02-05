#include <Arduino.h>
#include <EEPROM.h>
#include <LoRa.h>
#include <TinyGPS++.h>

#include "SdFunctions/SdFunctions.h"
#include "RadioFunctions/RadioFunctions.h"
#include "Log.h"

#define EEPROM_ID_ADDR 0

byte deviceId;

TinyGPSPlus gps;
int GPSBaud = 9600;
String data;
int Sat;
float Lat, LatOld, Long, LongOld, Alt, AltOld;
short StuckCount = 0;
bool beallt = 0;
bool nogp = 0;

void GpsSetup() {
  Serial.begin(9600);
  while(!Serial);

  Log::Success("Gps setup done");
}

String GpsSmartDelay(unsigned int ms)
{
  Log::Log("Entered SmartDelay");
  String rawData = "";
  unsigned long start = millis();

  do
  {
    while (Serial.available()){
      digitalWrite(LED_BUILTIN, HIGH);
      char rawChar = Serial.read();
      gps.encode(rawChar); //!!!!!!
      rawData += rawChar;
      digitalWrite(LED_BUILTIN, LOW);
    }
  } while (millis() - start < ms);
  return rawData;
}

String GpsMeasure(bool beal) {
  String _data;
  LatOld = Lat;
  LongOld = Long;
  AltOld = Alt;
  if (millis() > 500 && gps.charsProcessed() < 10) {
    Log::Error("No gps detected");
    nogp = 1;
  }
  Sat = gps.satellites.value();
  Lat = gps.location.lat();
  Long = gps.location.lng();
  Alt = gps.altitude.meters();
  if(!nogp){
    _data =  String(9) + ";" +
      String(Sat) + ";" +
      String(gps.time.hour()) + ":" +
      String(gps.time.minute()) + ":" +
      String(gps.time.second()) +  ";" +
      String(Lat, 6) + ";" +
      String(Long, 6) + ";" +
      String(Alt, 6);
    Log::Log("Data row constructed");  
  } else { _data = "No gps detected"; }
  if (beal == 1) {
    beallt = 1;
  } else if (Lat != 0 && Sat != 0) {
    beallt = 0;
  }
  return _data;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  SdFunctions::Setup(3);
  RadioFunctions::Setup(2,5,868E6);
  GpsSetup();
}

void loop() {
  String RawData = GpsSmartDelay(333);
  RadioFunctions::SendMessage("\t\t\t\t\t\t\t\t\t" + GpsMeasure(Sat != 0));

  delay(50);
}