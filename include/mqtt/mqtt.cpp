#include "mqtt.h"

namespace MQTT {

void mqtt::mqtt_event_handler_hangboardroutine(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{

    char *ourTaskName = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskName, "MQTT Up!\n");
    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : CONNECTED\n");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : DISCONNECTED\n");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : SUBSCRIBED\n");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : UNSUBSCRIBED\n");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : EVENT_PUBLISHED\n");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : EVENT_DATA\n");
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(ourTaskName, "MQTT_STATUS : ERROR\n");
        break;
    default:
        break;
    }
}

esp_mqtt_client_handle_t mqtt::mqtt_app_start(void)
{
    // Configure the MQTT broker and set the broker address
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address = {
                .uri = "mqtt://test.mosquitto.org",  // Broker URI
            }
        }
    };


    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt::mqtt_event_handler_hangboardroutine, NULL);
    esp_mqtt_client_start(client);

    return client;
}

}