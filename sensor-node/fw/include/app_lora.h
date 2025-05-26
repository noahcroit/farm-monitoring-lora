#ifndef _APP_LORA
#define _APP_LORA

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#if DEBUG == 1
#define debug(x)    Serial.print(x)
#define debugln(x)  Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define UART_SOF (char)0xAA
#define UART_EOF (char)0xBB

/* 
 * Struct for RA-01 LoRa module configuration 
 */
typedef struct {
    int node_id;
    uint8_t pin_mosi;
    uint8_t	pin_miso;
    uint8_t	pin_sck;
    uint8_t pin_cs;
    uint8_t pin_rst;
    uint8_t pin_irq;
    long freq;
    double bw; 
    uint8_t sf;
    uint8_t codingDenom;
    uint32_t preambleLen;
    uint8_t syncword;
    bool useCRC;

}app_lora_config_t;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Put non-name-mangling function prototypes in here
 *
 */
void app_lora_init(app_lora_config_t *cfg);
void app_lora_send_message(char *msg, int header);
bool app_lora_receive_message(char *msg, uint8_t *size);
void app_lora_pack_message(app_lora_config_t *cfg, char *msg, int moisture, float temp, float humid);

#ifdef __cplusplus
}
#endif

#endif
