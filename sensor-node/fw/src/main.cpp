#include <main.h>
#include <timerScheduler.h>
#include "esp_task_wdt.h"




// WDT timeout (in seconds)
#define WDT_TIMEOUT_SEC 10
// Period of task(s)
#define TASK_PERIOD_MS_SOIL 1000
#define TASK_PERIOD_MS_AIR  1000
#define TASK_PERIOD_MS_DISPLAY 1500
#define TASK_PERIOD_MS_LORA_TX 6100
#define TASK_PERIOD_MS_LORA_RX 200
#define TASK_PERIOD_MS_WDT_RESET 5000



// Task scheduler
TimerScheduler schd;
// Soil sensor by using ADC
SoilMoistureSense soil;
// Air sensor (temp + humid) by using I2C
TempHumidSense air;
// LoRa configuration
app_lora_config_t cfg_lora = {
    .node_id = (int)NODE_ID, 
    .pin_mosi = PIN_LORA_MOSI,
    .pin_miso = PIN_LORA_MISO,
    .pin_sck = PIN_LORA_SCK,
    .pin_cs = PIN_LORA_CS,
    .pin_rst = PIN_LORA_RST,      // LoRa radio reset
    .pin_irq = PIN_LORA_IRQ,      // change for your board; must be a hardware interrupt pin
    .freq = (long)433E6, // LoRa Frequency
    .bw = 125E3, 
    .sf = 7,
    .codingDenom = 5,
    .preambleLen = 8,
    .syncword = 0x12,
    .useCRC = false
};

int32_t moisture=0;
float temp=0;
float humid=0;

#if LORA_TX == 1
void task_soilmeasurement();
void task_airmeasurement();
void task_displayinfo();
void task_lora_tx();
#endif
#if LORA_RX == 1
void task_lora_rx();
#endif
void task_reset_wdt();

HardwareSerial Serial_LoRa(2);



void setup(){
    // Initialize UART
    Serial.begin(115200);
    Serial_LoRa.begin(9600, SERIAL_8N1, PIN_RXD2, PIN_TXD2);
    
    // Initialize LoRa module
    app_lora_init(&cfg_lora);
    debugln("LoRa initialize completed.");

    // Initialize soil moisture sensor
    soil.init();
    // Initialize air sensor (SHT31)
    air.init();
    debugln("Sensor initialization completed.");
   
    // Initialize Watchdog (WDT)
    esp_task_wdt_init(WDT_TIMEOUT_SEC, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL); //add current thread to WDT watch

    // Create Tasks
    // and Add task to the scheduller
#if LORA_TX == 1
#define TASK_PERIOD_MS_SOIL 1000
    PeriodTask t1(TASK_PERIOD_MS_SOIL, &task_soilmeasurement); 
    schd.addTask(t1);
    PeriodTask t2(TASK_PERIOD_MS_AIR, &task_airmeasurement); 
    schd.addTask(t2);
    PeriodTask t3(TASK_PERIOD_MS_DISPLAY, &task_displayinfo); 
    schd.addTask(t3);
    PeriodTask t4(TASK_PERIOD_MS_LORA_TX, &task_lora_tx); 
    schd.addTask(t4);
#endif
#if LORA_RX == 1
    PeriodTask t5(TASK_PERIOD_MS_LORA_RX, &task_lora_rx); 
    schd.addTask(t5);
#endif
    PeriodTask t6(TASK_PERIOD_MS_WDT_RESET, &task_reset_wdt); 
    schd.addTask(t6);
}



void loop(){
    /*
     * Run task scheduler
     * This will run all tasks and it is a blocking loop.
     * scheduler will not stop unless schd.stop() is called using Interrupt technique to yield the scheduler.
     *
     */
    schd.start();

    /*
     * Code section when scheduler stopped run. 
     *
     */
    setup();
}



void task_soilmeasurement() {
    /*
     * Read the soil moisture value from sensor
     *
     */
    moisture = soil.getValue();
}

void task_airmeasurement() {
    // read SHT31 params
    temp = air.getTemp();
    humid = air.getRH();
}

void task_displayinfo() {
    debug("Moisture=");
    debug(moisture);
    debug(",temp=");
    debug(temp);
    debug(",RH=");
    debugln(humid);
}

void task_lora_tx() {
    char tx_msg[64];
    int header=0;
    /*
     * Pack the message and send it to the LoRa gateway
     *
     */
    app_lora_pack_message(&cfg_lora, tx_msg, moisture, temp, humid);
    app_lora_send_message(tx_msg, header);
    debug("Sending LoRa message, msg=");
    debugln(tx_msg);
}

void task_lora_rx() {
    char rx_msg[64];
    uint8_t rx_size=0;

    /*
     * Polling the LoRa buffer whether the data is received or not.
     * Then, read the received LoRa message
     */
    if (app_lora_receive_message(rx_msg, &rx_size)) {
        debug("LoRa received data size = ");
        debug(rx_size);
        debug(", packet='");
        debug(rx_msg);
        debugln("'");
        Serial_LoRa.print(rx_msg);
    }
}

void task_reset_wdt() {
    esp_task_wdt_reset();
}
