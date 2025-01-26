#include "helper.h"


void Main::wifi_run()
{

    char *ourTaskName = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskName, "Starting Up!\n");
    
    wifiState = Wifi.GetState();

    switch (wifiState)
    {
    case WIFI::Wifi::state_e::READY_TO_CONNECT:
        ESP_LOGI(ourTaskName,"Wifi Status: READY_TO_CONNECT\n");
        Wifi.Begin();
        break;
    case WIFI::Wifi::state_e::DISCONNECTED:
        ESP_LOGI(ourTaskName,"Wifi Status: DISCONNECTED\n");
        Wifi.Begin();
        break;
    case WIFI::Wifi::state_e::CONNECTING:
        ESP_LOGI(ourTaskName,"Wifi Status: CONNECTING\n");
        break;
    case WIFI::Wifi::state_e::WAITING_FOR_IP:
        ESP_LOGI(ourTaskName,"Wifi Status: WAITING_FOR_IP\n");
        break;;
    case WIFI::Wifi::state_e::ERROR:
        ESP_LOGI(ourTaskName,"Wifi Status: ERROR\n");
        break;
    case WIFI::Wifi::state_e::CONNECTED:
        ESP_LOGI(ourTaskName,"Wifi Status: CONNECTED\n");
        break;
    case WIFI::Wifi::state_e::NOT_INITIALIZED:
        ESP_LOGI(ourTaskName,"Wifi Status: NOT_INITIALIZED\n");
        break;
    case WIFI::Wifi::state_e::INITIALIZED:
        ESP_LOGI(ourTaskName,"Wifi Status: INITIALIZED\n");
        break;
    }
}

void Main::wifi_setup(){

    esp_event_loop_create_default();
    nvs_flash_init();

    Wifi.SetCredentials("WIFI", "PASSWORD");
    Wifi.Init();

}