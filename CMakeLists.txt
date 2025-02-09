set(WIFI_DIR ../include/wifi_connectivity)
set(MQTT_DIR ../include/mqtt)
set(MQTT_CPP ../include/mqtt/mqtt.cpp)
set(WIFI_CPP ../include/wifi_connectivity/wifi.cpp)
set(HELPER_CPP ../main/helper.cpp)
  
idf_component_register(SRCS Hangboard.cpp ${HELPER_CPP} ${WIFI_CPP} ${MQTT_CPP}
                    REQUIRES esp_wifi nvs_flash esp_event esp_netif lwip efuse esp_driver_gpio
                    REQUIRES mqtt nvs_flash esp_netif
                    INCLUDE_DIRS "." ${WIFI_DIR} ${MQTT_DIR} ${NLOHMANN_JSON})

