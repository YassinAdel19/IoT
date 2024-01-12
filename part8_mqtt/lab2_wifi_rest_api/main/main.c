/****************************************************************************
 * Copyright (C) 2021 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file main.c
 * @author Fabrice Muller
 * @date 12 Nov. 2021
 * @brief File containing the lab1-1 of Part 7.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "my_helper_fct.h"
#include "esp_http_client.h"
#include "wifi_connect.h"
#include "http_data.h"

#include "wifi_connect.h"
#include "http_data.h"
//#include "ntp_time.h"
#include "mqtt_tcp.h"


 /* To be added */
 #include "esp_http_client.h"
 
static const uint32_t STACK_SIZE = 3*1024;

static const uint32_t T1_PRIO = 5;


#include "cJSON.h"



/**
 * @brief Starting point function
 * 
 */

void ConnectedWifi(void *pvParameters);

static const char *TAG = "MQTT_MAIN";

void testMqttTask(void *para);


void app_main() {
  /* ERROR, WARNING, INFO level log */
  esp_log_level_set(TAG, ESP_LOG_INFO);
  
  /* Init WiFi */
  mqtt_start("mqtt://127.0.0.1");

  vTaskSuspendAll();
  /* Create connected WiFi Task, STACK=3*1024, Priority=5 */
  xTaskCreate(ConnectedWifi,	"Connected Wifi", STACK_SIZE,	(void*)"Connected Wifi", T1_PRIO,	NULL);
  
  xTaskResumeAll();

  /* Delete task */
  vTaskDelete(NULL);
}


void ConnectedWifi(void *pvParameterts){

  for(;;){
    if (xSemaphoreTake(getConnectionMqttSemaphore(), pdMS_TO_TICKS(10000)) == pdTRUE){
      DISPLAY("Connected on SSID");
      DISPLAY("Run App");


      if (xSemaphoreTake(getConnectionMqttSemaphore(),portMAX_DELAY)){
        DISPLAY("Retired on SSID");
      }
    }
    else{
      DISPLAY("Failed to connect. Retry in");
      for (int i=0; i<=5; i++){
        DISPLAY("%d",i);
        vTaskDelay(pdMS_TO_TICKS(1000));
      }

      esp_restart();
    }
  }
}


void testMqttTask(void *para) {

  /* Get MQTT Client passed by task parameter */
  esp_mqtt_client_handle_t mqtt_client = (esp_mqtt_client_handle_t)para;

  /* Subscription of the End-Node : Sub_AREA1 */
  // esp_mqtt_client_subscribe()


  /* Publishing temperature with the End-Node : Pub_E110_Room */
  // esp_mqtt_client_publish()


  /* Wait 3 sec. */

  /* Publishing humidity AND temperature with the End-Node : Pub_Sxxx_Room */


  /* Subscription of the End-Node : Sub_E110 */


  /* Publishing temperature with the End-Node : Pub_E110_Room */


  /* Wait 3 sec. */

  /* Publishing humidity AND temperature with the End-Node : Pub_Sxxx_Room */


  /* Unsubscribe of the End-Node : Sub_AREA1 */
  // esp_mqtt_client_unsubscribe()


  /* Publishing temperature with the End-Node : Pub_E110_Room */


  /* Wait 3 sec. */

  /* Publishing humidity with the End-Node : Pub_Sxxx_Room */


  vTaskDelete(NULL);
}