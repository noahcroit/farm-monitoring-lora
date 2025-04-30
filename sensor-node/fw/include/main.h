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
#include "app_soilmoisture.h"
#include "app_lora.h"

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
