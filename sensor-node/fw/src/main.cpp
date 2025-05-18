#include <main.h>
#include <timerScheduler.h>



// Task scheduler
TimerScheduler schd;
// Soil sensor by using ADC
SoilMoistureSense soil;
// LoRa configuration
app_lora_config_t cfg_lora = {
    .pin_mosi = 23,
    .pin_miso = 19,
    .pin_sck = 18,
    .pin_cs = 5,
    .pin_rst = 15,      // LoRa radio reset
    .pin_irq = 2,       // change for your board; must be a hardware interrupt pin
    .freq = (long)433E6 // LoRa Frequency
};

int32_t moisture;

#if LORA_TX == 1
void task_soilmeasurement();
void task_airmeasurement();
void task_lora_tx();
#endif
#if LORA_RX == 1
void task_lora_rx();
#endif



void setup(){
    // Initialize UART
    Serial.begin(115200);
    
    // Initialize LoRa module
    app_lora_init(&cfg_lora);
    debugln("LoRa initialize completed.");

    // Initialize soil moisture sensor
    soil.init();
    debugln("Soil moisture initialize completed.");
    
    // Create Tasks
    // and Add task to the scheduller
#if LORA_TX == 1
    PeriodTask t1(3000, &task_soilmeasurement); 
    schd.addTask(t1);
    PeriodTask t2(5000, &task_airmeasurement); 
    schd.addTask(t2);
    PeriodTask t3(10000, &task_lora_tx); 
    schd.addTask(t3);
#endif
#if LORA_RX == 1
    PeriodTask t4(200, &task_lora_rx); 
    schd.addTask(t4);
#endif
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
    debug("Moisture=");
    debugln(moisture);
}

void task_airmeasurement() {

}

void task_lora_tx() {
    char tx_msg[32];
    /*
     * Pack the message and send it to the LoRa gateway
     *
     */
    sprintf(tx_msg, "soil moisture = %d\n", moisture);
    debug("Sending LoRa message, msg=");
    debugln(tx_msg);
    app_lora_send_message(tx_msg, false);
}

void task_lora_rx() {
    char rx_msg[32];
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
    }
}
