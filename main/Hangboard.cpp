#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "driver/gpio.h"
#include <string>
#include "helper.h"

Main wifi_app;

void send_data(){}

void beep(int milliseconds_to_beep, int milliseconds_to_quiet, gpio_num_t sound_output) {
    
    gpio_set_level(sound_output,1);
    vTaskDelay (milliseconds_to_beep / portTICK_PERIOD_MS);
    gpio_set_level(sound_output,0);
    vTaskDelay (milliseconds_to_quiet / portTICK_PERIOD_MS);
}

void hanging_routine(gpio_num_t busy_led_output, gpio_num_t idle_led_output, gpio_num_t sound_output){

    gpio_set_level(idle_led_output,0);

    // Wait 5 seconds for climber to get in position and ready
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    // Coundown for hanging
    for (int i=0; i<3;i++){
        beep(150,500,sound_output);
    }
    beep(500,0,sound_output);
 
    // 10 second hang
    gpio_set_level(busy_led_output,1);
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    gpio_set_level(busy_led_output,0);

    // Sound to indicate end 

    gpio_set_level(sound_output,1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(sound_output,0);

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

    gpio_num_t four_finger_input = static_cast<gpio_num_t>(33); // GPIO5 as input
    gpio_num_t three_finger_input = static_cast<gpio_num_t>(32); // GPIO5 as input
    gpio_num_t sound_output = static_cast<gpio_num_t>(2); // Sound as output
    gpio_num_t busy_led_output = static_cast<gpio_num_t>(4); // LED when the code is running
    gpio_num_t idle_led_output = static_cast<gpio_num_t>(15); // LED when the system is idle

    // Reset GPIO pins
    gpio_reset_pin(four_finger_input);
    gpio_reset_pin(three_finger_input);
    gpio_reset_pin(busy_led_output);
    gpio_reset_pin(idle_led_output);
    gpio_reset_pin(sound_output);

    // Configure GPIO directions
    gpio_set_direction(busy_led_output, GPIO_MODE_OUTPUT);
    gpio_set_direction(idle_led_output, GPIO_MODE_OUTPUT)
    gpio_set_direction(sound_output, GPIO_MODE_OUTPUT);
    gpio_set_direction(four_finger_input, GPIO_MODE_INPUT);
    gpio_set_direction(three_finger_input, GPIO_MODE_INPUT);

    // Enable pull-down resistor on the input pin
    gpio_pullup_dis(four_finger_input);   // Disable pull-up resistor
    gpio_pulldown_en(four_finger_input);  // Enable pull-down resistor
    gpio_pullup_dis(three_finger_input);   // Disable pull-up resistor
    gpio_pulldown_en(three_finger_input);  // Enable pull-down resistor

    // Set initial output level
    gpio_set_level(sound_output, 0);
    gpio_set_level(busy_led_output, 0);
    gpio_set_level(idle_led_output, 0)

    MQTT::mqtt mqtt_broker;

    esp_mqtt_client_handle_t created_client = mqtt_broker.mqtt_app_start();

    while (true) {

        if(gpio_get_level(four_finger_input)==0){
            gpio_set_level(sound_output,0);
            gpio_set_level(busy_led_output, 0);
            gpio_set_level(idle_led_output, 1);
        }

        else if((gpio_get_level(four_finger_input)==1)){
            hanging_routine(busy_led_output,idle_led_output,sound_output);
            publish_hangboard_data(GRIP_TYPES::FOUR_FINGER_DRAG,created_client);
        }
        else if((gpio_get_level(three_finger_input)==1)){
            hanging_routine(busy_led_output,idle_led_output,sound_output);
            publish_hangboard_data(GRIP_TYPES::THREE_FINGER_DRAG,created_client);
        }
    }
}