#ifndef _APP_SHT31
#define _APP_SHT31

#define FRAMEWORK_ARDUINO   0
#define FRAMEWORK_STM32CUBE 1
#define FRAMEWORK_ZEPHYR    2
#define SELECTED_FRAMEWORK  FRAMEWORK_ARDUINO

#if SELECTED_FRAMEWORK == FRAMEWORK_ARDUINO
#include <Arduino.h>
#include <Wire.h>
#endif

/* 
 * Struct for BQ25730 Configuration 
 * - I2C configuration
 * - GPIO mapping
 * - Chip configuration
 */
typedef struct
{
    uint32_t i2c_freq;
    uint8_t	pin_i2c_sda;
    uint8_t	pin_i2c_scl;
    uint8_t dev_addr;
    uint8_t adc_mode;
    uint8_t watchdog_adj;
    uint8_t rsr;
    uint8_t rac;
    float vsysmin;
    float vcharge;
    float icharge;

}bq25730_config_t;

// Choose to use delay() based on framework
#if SELECTED_FRAMEWORK == FRAMEWORK_ARDUINO
#define delay(x)    delay(x)
#elif SELECTED_FRAMEWORK == FRAMEWORK_STM32CUBE
#define delay(x)    HAL_Delay(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Put non-name-mangling function prototypes in here
 *
 */

#ifdef __cplusplus
}
#endif

#endif
