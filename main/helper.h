#include <string>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "wifi.h"

class Main final
{
private:
public:
    void wifi_run(void);
    void wifi_setup(void);

    WIFI::Wifi::state_e wifiState { WIFI::Wifi::state_e::NOT_INITIALIZED };
    WIFI::Wifi Wifi;
};