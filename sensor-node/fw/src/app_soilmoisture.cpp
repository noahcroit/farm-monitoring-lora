#include "app_soilmoisture.h"




SoilMoistureSense::SoilMoistureSense(unsigned int adc_pin, uint8_t adc_resolution) {
    __adc_pin = adc_pin;
    __adc_resolution = adc_resolution;
    __en_flag = false;
}

SoilMoistureSense::SoilMoistureSense() {
    __adc_pin = ADC_PIN_DEFAULT;
    __adc_resolution = ADC_RESOLUTION_DEFAULT;
    __en_flag = false;

}

void SoilMoistureSense::init() {
    adc_set_resolution(12);
    adc_set_hw_resolution(__adc_resolution);
    enable();
}

void SoilMoistureSense::enable() {
    // enable ADC and attach to PIN 
    __en_flag = true;
}

void SoilMoistureSense::disable() {
    // disable ADC and detach to PIN 
    __en_flag = false;
}

int32_t SoilMoistureSense::getValue() {
    int32_t value=0;
    if (__en_flag) {
        value = adc_read(__adc_pin);
    }
    return value;
}

