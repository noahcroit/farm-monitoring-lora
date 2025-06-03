#ifndef _MAIN_H
#define _MAIN_H

// Choose to use Serial.print() for debugging or not
#if DEBUG == 1
#define debug(x)    Serial.print(x)
#define debugln(x)  Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include "app_soilmoisture.h"
#include "app_temphumid.h"
#include "app_lora.h"

#define PIN_LORA_MOSI 23
#define PIN_LORA_MISO 19
#define PIN_LORA_SCK 18
#define PIN_LORA_CS 5
#define PIN_LORA_RST 15
#define PIN_LORA_IRQ 2
#define PIN_RXD2 16
#define PIN_TXD2 17
#define PIN_SHT31_SCL 22
#define PIN_SHT31_SDA 21

#ifdef __cplusplus
extern "C" {
#endif
/*
 * function prototypes which don't want cpp compiler to do name-mangling
 *
 *
 */

#ifdef __cplusplus
}
#endif
#endif
