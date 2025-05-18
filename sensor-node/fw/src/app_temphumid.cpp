#include "app_temphumid.h"



/*
TempHumidSense::TempHumidSense() {
    __en_flag = false;
    Adafruit_SHT31 __dev = Adafruit_SHT31();
}
*/

TempHumidSense::TempHumidSense() {
    __en_flag = false;
    Adafruit_SHT31 __dev = Adafruit_SHT31();
    __addr = 0x44;
}

int TempHumidSense::init() {
    enable();
    if(! __dev.begin(__addr)) {
        return -1;
    }
    else{
        return 0;
    }   
}

void TempHumidSense::enable() {
    __en_flag = true;
}

void TempHumidSense::disable() {
    __en_flag = false;
}

float TempHumidSense::getTemp() {
    float value=0;
    if (__en_flag) {
        //value = adc_read(__adc_pin);
        value = __dev.readTemperature();
    }
    return value;
}

float TempHumidSense::getRH() {
    float value=0;
    if (__en_flag) {
        //value = adc_read(__adc_pin);
        value = __dev.readHumidity();
    }
    return value;
}
