#ifndef _APP_SOILMOISTURE
#define _APP_SOILMOISTURE

#include <Arduino.h>
#define adc_read(x) analogRead(x)
#define adc_set_resolution(x) analogReadResolution(x)
#define adc_set_hw_resolution(x) analogSetWidth(x)

#define ADC_PIN_DEFAULT 34
#define ADC_RESOLUTION_DEFAULT 12

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

class SoilMoistureSense {
private:
    bool __en_flag;
    unsigned int __adc_pin;
    uint8_t __adc_resolution;

public:
    SoilMoistureSense(unsigned int adc_pin, uint8_t adc_resolution);
    SoilMoistureSense();
    void init();
    void enable();
    void disable();
    int32_t getValue();
};

#ifdef __cplusplus
}
#endif

#endif
