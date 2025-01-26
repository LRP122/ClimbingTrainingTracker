set(WIFI_DIR ../include/wifi_connectivity)
set(WIFI_CPP ../include/wifi_connectivity/wifi.cpp)
set(HELPER_CPP ../main/helper.cpp)
                    
idf_component_register(SRCS Hangboard.cpp ${HELPER_CPP} ${WIFI_CPP}
                    REQUIRES esp_wifi nvs_flash esp_event esp_netif lwip efuse esp_driver_gpio
                    INCLUDE_DIRS "." ${WIFI_DIR})
