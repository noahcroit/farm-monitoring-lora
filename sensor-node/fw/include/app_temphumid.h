#ifndef _APP_TEMPHUMID
#define _APP_TEMPHUMID

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

#if DEBUG == 1
#define debug(x)    Serial.print(x)
#define debugln(x)  Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Put non-name-mangling function prototypes in here
 *
 */

class TempHumidSense {
private:
    bool __en_flag;
    uint8_t __addr;
    Adafruit_SHT31 __dev;

public:
    TempHumidSense();
    TempHumidSense(uint8_t addr);
    int init();
    void enable();
    void disable();
    float getTemp();
    float getRH();
};

#ifdef __cplusplus
}
#endif

#endif
