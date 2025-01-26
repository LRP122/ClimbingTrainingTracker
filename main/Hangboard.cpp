#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include <string>
#include "helper.h"

Main wifi_app;

void send_data(){}

void beep(int milliseconds_to_beep, int milliseconds_to_quiet, gpio_num_t SOUND_OUTPUT) {
    
    gpio_set_level(SOUND_OUTPUT,1);
    vTaskDelay (milliseconds_to_beep / portTICK_PERIOD_MS);
    gpio_set_level(SOUND_OUTPUT,0);
    vTaskDelay (milliseconds_to_quiet / portTICK_PERIOD_MS);
}

void hanging_routine(gpio_num_t LED_OUTPUT, gpio_num_t SOUND_OUTPUT){

    // Wait 5 seconds for climber to get in position and ready
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    // Coundown for hanging
    for (int i=0; i<3;i++){
        beep(150,500,SOUND_OUTPUT);
    }
    beep(500,0,SOUND_OUTPUT);
 
    // 10 second hang
    gpio_set_level(LED_OUTPUT,1);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_set_level(LED_OUTPUT,0);

    // Sound to indicate end 

    gpio_set_level(SOUND_OUTPUT,1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(SOUND_OUTPUT,0);

}

extern "C" void app_main()
{

    WIFI::Wifi Wifi;
    wifi_app.wifi_setup();

    char *ourTaskName = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskName, "Starting Up!\n");

    while(Wifi.GetState() != WIFI::Wifi::state_e::CONNECTED){
        vTaskDelay(pdMS_TO_TICKS(1000));
        wifi_app.wifi_run();
    }

    gpio_num_t INPUT = static_cast<gpio_num_t>(15); // GPIO5 as input
    gpio_num_t SOUND_OUTPUT = static_cast<gpio_num_t>(2); // Sound as output
    gpio_num_t LED_OUTPUT = static_cast<gpio_num_t>(4); // LED

    // Reset GPIO pins
    gpio_reset_pin(INPUT);
    gpio_reset_pin(LED_OUTPUT);
    gpio_reset_pin(SOUND_OUTPUT);

    // Configure GPIO directions
    gpio_set_direction(LED_OUTPUT, GPIO_MODE_OUTPUT);
    gpio_set_direction(SOUND_OUTPUT, GPIO_MODE_OUTPUT);
    gpio_set_direction(INPUT, GPIO_MODE_INPUT);

    // Enable pull-down resistor on the input pin
    gpio_pullup_dis(INPUT);   // Disable pull-up resistor
    gpio_pulldown_en(INPUT);  // Enable pull-down resistor

    // Set initial output level
    gpio_set_level(SOUND_OUTPUT, 0);
    gpio_set_level(LED_OUTPUT, 0);

    while (true) {

        if(gpio_get_level(INPUT)==0){
            gpio_set_level(SOUND_OUTPUT,0);
            gpio_set_level(LED_OUTPUT, 0);

        }
        else if((gpio_get_level(INPUT)==1)){
            hanging_routine(LED_OUTPUT,SOUND_OUTPUT);
            
            // Send data to MQTT client to update the table to put in what kind of hang on what day
        }
    }
}