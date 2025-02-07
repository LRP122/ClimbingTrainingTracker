#include <string>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi.h"
#include "mqtt.h"

#include "json.hpp"

class Main final
{
private:
public:
    void wifi_run(void);
    void wifi_setup(void);

    WIFI::Wifi::state_e wifiState { WIFI::Wifi::state_e::NOT_INITIALIZED };
    WIFI::Wifi Wifi;
};

enum GRIP_TYPES {
    FOUR_FINGER_DRAG,
    FOUR_FINGER_HALFCRIMP,
    THREE_FINGER_DRAG,
    THREE_FINGER_HALFCRIMP
};

void publish_hangboard_data(GRIP_TYPES grip_type, esp_mqtt_client_handle_t client);