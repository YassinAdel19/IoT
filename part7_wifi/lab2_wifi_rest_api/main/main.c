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

static const char *TAG = "WIFI_LAB";

 /* To be added */
 #include "esp_http_client.h"
 
static const uint32_t STACK_SIZE = 3*1024;

static const uint32_t T1_PRIO = 5;


#include "cJSON.h"


/* openweathermap API URL for Cannes city, Unit = degree */
const char *CITY = "Cannes";
const char *OPEN_WEATHER_MAP_URL = "api.openweathermap.org/data/2.5/weather?q=Cannes&appid=bfaf90865d45e39c390da17ffa61e195";

/* Example of response for testing the extractJSONWeatherMapInformation() function */
const char *RESP_EXAMPLE = "{\"coord\":{\"lon\":7.0167,\"lat\":43.55},\"weather\":[{\"id\":800,\"main\":\"Clear\",\"description\":\"clear sky\",\"icon\":\"01d\"}],\"base\":\"stations\",\"main\":{\"temp\":24.72,\"feels_like\":24.84,\"temp_min\":23.12,\"temp_max\":25.74,\"pressure\":1019,\"humidity\":61},\"visibility\":10000,\"wind\":{\"speed\":3.6,\"deg\":170},\"clouds\":{\"all\":0},\"dt\":1633099464,\"sys\":{\"type\":1,\"id\":6507,\"country\":\"FR\",\"sunrise\":1633066158,\"sunset\":1633108421},\"timezone\":7200,\"id\":6446684,\"name\":\"Cannes\",\"cod\":200}";

/* Sensor information */
# define WEATHERMAPINFO_DESCRIPTION_LENGTH 100

typedef struct {
  float latitude;
  float longitude;
  float temp;
  float feels_like;
  float temp_min;
  float temp_max;
  char description[WEATHERMAPINFO_DESCRIPTION_LENGTH];
} weathermapinfo_t;

void extractJSONWeatherMapInformation(char *resp, weathermapinfo_t *weathermapinfo);

/**
 * @brief Starting point function
 * 
 */

void ConnectedWifi(void *pvParameters);


void app_main() {
  /* ERROR, WARNING, INFO level log */
  esp_log_level_set(TAG, ESP_LOG_INFO);
  
  /* Init WiFi */
  wifiInit();

  vTaskSuspendAll();
  /* Create connected WiFi Task, STACK=3*1024, Priority=5 */
  xTaskCreate(ConnectedWifi,	"Connected Wifi", STACK_SIZE,	(void*)"Connected Wifi", T1_PRIO,	NULL);
  
  xTaskResumeAll();

  /* Delete task */
  vTaskDelete(NULL);
}

/**
 * @brief Parse the JSON Open Weather Map Information and extract useful information to print it.
 * 
 * @param resp textual response of the Open Weather Map server 
 */
void extractJSONWeatherMapInformation(char *resp, weathermapinfo_t *weathermapinfo) {

    /* Convert textual resp to JSON object */
    cJSON *payload = cJSON_Parse(resp);

    /* Coordonate (JSon Items)43.550000,7.016700 */
    cJSON *coord = cJSON_GetObjectItem(payload, "coord");   
    cJSON *longitude = cJSON_GetObjectItem(coord, "lon");
    cJSON *latitude = cJSON_GetObjectItem(coord, "lat");

    cJSON *weather = cJSON_GetObjectItem(payload, "main");   
    cJSON *temp = cJSON_GetObjectItem(weather, "temp");   
    cJSON *feels_like = cJSON_GetObjectItem(weather, "feels_like"); 
    cJSON *temp_min = cJSON_GetObjectItem(weather, "temp_min"); 
    cJSON *temp_max = cJSON_GetObjectItem(weather, "temp_max"); 


    /* Set information in the structure */
    weathermapinfo->latitude = latitude->valuedouble;
    weathermapinfo->longitude = longitude->valuedouble;    

    weathermapinfo->temp = temp->valuedouble;
    weathermapinfo->feels_like = feels_like->valuedouble;   
    weathermapinfo->temp_min = temp_min->valuedouble;
    weathermapinfo->temp_max = temp_max->valuedouble;   

    /* Free memory */
    cJSON_Delete(payload);
}

void ConnectedWifi(void *pvParameterts){
  http_param_t *param = malloc(sizeof(http_param_t));
  for(;;){
    if (xSemaphoreTake(getConnectionWifiSemaphore(), pdMS_TO_TICKS(10000)) == pdTRUE){
      DISPLAY("Connected on SSID");
      DISPLAY("Run App");

      char *url = malloc(strlen(OPEN_WEATHER_MAP_URL) + /*strlen(CITY)*/ + 1);
      sprintf(url, OPEN_WEATHER_MAP_URL, CITY);

      fetchHttpData(param, "http://api.openweathermap.org/data/2.5/weather?q=Cannes&appid=bfaf90865d45e39c390da17ffa61e195");
      printf("%.*s", param->index, (char*)param->buffer);

      //free(param);
      free(url);

      /* Extract openweathermap information from response */
      weathermapinfo_t weathermapinfo;
      extractJSONWeatherMapInformation((char*)param->buffer, &weathermapinfo);

      printf("METEO at %s\n", CITY);
      printf("(latitude,longitude) = (%f,%f)\n", weathermapinfo.latitude, weathermapinfo.longitude);
      printf("(temp,real_feal, temp_min, temp_max) = (%f,%f,%f,%f)\n", weathermapinfo.temp, weathermapinfo.feels_like, weathermapinfo.temp_min, weathermapinfo.temp_max);

      if (xSemaphoreTake(getConnectionWifiSemaphore(),portMAX_DELAY)){
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





