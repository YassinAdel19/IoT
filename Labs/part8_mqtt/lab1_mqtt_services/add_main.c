/****************************************************************************
 * Copyright (C) 2021 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file add_main.c
 * @author Fabrice Muller
 * @date 12 Nov. 2021
 * @brief File containing the lab1 of Part 8.
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
#include "esp_http_client.h"
#include "esp_sntp.h"
#include "cJSON.h"

#include "wifi_connect.h"
#include "http_data.h"
#include "ntp_time.h"
#include "mqtt_tcp.h"

static const char *TAG = "MQTT_MAIN";

void testMqttTask(void *para);

/**
 * @brief Wifi Task example to use the wifi connection 
 * 
 * @param para parameter of the task
 */

void connectedWifiTask(void *para) {

  ...

      /* Run application */
      printf("Run Application\n");

      /* Start MQTT client */
      

      /* Get MQTT connection event (semaphore) */


      /* Wait on MQTT Connected event */

      /* If no error
       * Create Test Task (testMqttTask), priority = 6, parameter = MQTT Client, stack size = 3*1024
       */
    

      /* Blocked until loss connection and retry connection */
      xSemaphoreTake(connectionWifiSem, portMAX_DELAY);
    ...
}

/**
 * @brief Starting point function
 * 
 */


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



