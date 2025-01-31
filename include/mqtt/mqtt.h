#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "esp_log.h"
#include "mqtt_client.h"

namespace MQTT {

    class mqtt {
    
    public:

    static void mqtt_event_handler_hangboardroutine(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    esp_mqtt_client_handle_t mqtt_app_start();

    };

}