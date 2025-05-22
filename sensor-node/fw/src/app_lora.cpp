#include "app_lora.h"




void app_lora_init(app_lora_config_t *cfg) {
    
    pinMode(cfg->pin_cs, OUTPUT);
    pinMode(cfg->pin_rst, OUTPUT);
    pinMode(cfg->pin_irq, INPUT);
    
    SPI.begin(  cfg->pin_sck,
                cfg->pin_miso, 
                cfg->pin_mosi, 
                cfg->pin_cs);

    LoRa.setPins(   cfg->pin_cs,
                    cfg->pin_rst,
                    cfg->pin_irq);

    LoRa.setSPI(SPI);

    while (!LoRa.begin(cfg->freq)) {
        debug(".");
        delay(100); 
    }
    LoRa.setSpreadingFactor(cfg->sf);
    LoRa.setSignalBandwidth(cfg->bw);
    LoRa.setCodingRate4(cfg->codingDenom);
    LoRa.setPreambleLength(cfg->preambleLen);
    LoRa.setSyncWord(cfg->syncword);
    if (cfg->useCRC) {
        LoRa.enableCrc();
    }
    else {
        LoRa.disableCrc();
    } 
}

void app_lora_send_message(char *msg, int header) {
    // send packet
    LoRa.beginPacket(header);
    LoRa.print(msg);
    LoRa.endPacket();
}

bool app_lora_receive_message(char *msg, uint8_t *size) {
    uint8_t packetSize = LoRa.parsePacket();
    if (packetSize) {
        char* packet = (char*)malloc((packetSize + 1) * sizeof(char));
        char LoRaByte;
        int i=0;
        // read packet
        while (LoRa.available()) {
            if (i < packetSize) {
                LoRaByte = LoRa.read();
                *(packet + i) = LoRaByte;
                i++;
            }
        }
        *(packet + packetSize) = '\0';
        *size = packetSize + 1;
        memcpy(msg, (const void *)packet, packetSize + 1);
        free(packet);
        return true;
    }
    return false;
}
