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

void publish_hangboard_data(GRIP_TYPES grip_type, esp_mqtt_client_handle_t client)
{

    nlohmann::json_abi_v3_11_3::json hangboard_data;
    hangboard_data["tfd"] = 0;
    hangboard_data["tfc"] = 0;
    hangboard_data["ffd"] = 0;
    hangboard_data["ffc"] = 0;


    switch(grip_type){
        case THREE_FINGER_DRAG:
            hangboard_data["tfd"] = 1;
            break;
        case THREE_FINGER_HALFCRIMP:
            hangboard_data["tfc"] = 1;
            break;
        case FOUR_FINGER_DRAG:
            hangboard_data["ffd"] = 1;
            break;
        case FOUR_FINGER_HALFCRIMP:
            hangboard_data["ffc"] = 1;
            break;
    }

    std::string hangboard_data_str = hangboard_data.dump();
    const char* hangboard_data_char = hangboard_data_str.c_str();
    esp_mqtt_client_publish(client, "/climbing/hangboard", hangboard_data_char , 0, 1, 0);


}