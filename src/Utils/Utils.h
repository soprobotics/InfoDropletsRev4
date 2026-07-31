#pragma once

#include <Arduino.h>

struct DropletData{
    const static long R_freq = 868E6;
    const static byte R_dio0 = 6;
    const static byte R_rst = 5;
    const static byte R_ss = 2;
    const static byte SD_ss = 3;
    const static int DROPLET_ID_EEPROM_ADDR = 0;
    const static constexpr size_t READ_BUFFER_SIZE = 50; //longest possible measurement: 90;10;17:28:40;-48.268917;-116.403818;-17000.01 --> 47 chars + 2 safety + 1 null-terminator = 50

};

struct StartupData{
    bool radioSetupOk;
    bool sdSetupOk;
    bool gpsSetupOk;
    bool gpsHighAltOk;
    byte Version;
};

namespace Utils{
    void SetupSSPins();
    void SelectSD();
    void SelectRadio();

    bool WriteUInt(char*& p, char* end, unsigned value);
    bool WriteFixed6(char*& p, char* end, float value);
    bool WriteFixed2(char*& p, char* end, float value);
    
    char* IntToStr(int value, char* p, char* end);   
}