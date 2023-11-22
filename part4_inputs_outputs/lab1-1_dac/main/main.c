/****************************************************************************
 * Copyright (C) 2021 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file main.c
 * @author Fabrice Muller
 * @date 31 Oct. 2021
 * @brief File containing the lab1 of Part 4.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_timer.h"
#include "esp_sleep.h"

#include "driver/gpio.h"
#include "driver/dac.h"


/* 
--------------------
Digital to Analog Converter (DAC)

Documentation:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html


--------------------
High Resolution Timer (esp_timer)
 
Documentation:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/esp_timer.html

*/

/* Default stack size for tasks */
static const uint32_t STACK_SIZE = 4000;
static const uint32_t T2_PRIO = 6;


#define TRIANGLE_PERIOD_1 5000000 // 5 seconds in microseconds

/**
 * @brief Starting point function
 * 
 */

 void triangular_waveform(void *pvParameters){
	uint32_t amplitude = 0;
    uint32_t increment = 1;

	for(;;){
		dac_output_voltage(DAC_CHANNEL_1, amplitude);
		amplitude += increment;

		if (amplitude == 255 || amplitude == 0) {
            increment = -increment;
        }
	}
	vTaskDelete(NULL);
 }

void app_main(void) {

	/**************************************************/
	/* Configure DAC (Digital to Analog Converter)    */

	//DAC_CHANNEL_1 = GPIO25 (IO25); 
	dac_output_enable(DAC_CHANNEL_1);

	xTaskCreate(triangular_waveform,	/* Pointer to the function that implements the task. */
		"Task",				/* Text name for the task.  This is to facilitate debugging only. */
		STACK_SIZE,				/* Stack depth  */
		(void*)"Task",		/* Pass the text to be printed in as the task parameter. */
		T2_PRIO,				/* Task priority */
		NULL);					/* We are not using the task handle. */

	vTaskDelay(TRIANGLE_PERIOD_1 / portTICK_PERIOD_MS);




	/**************************************************/
	/* Configure Timer                                */
    const esp_timer_create_args_t esp_timer_create_args = {
        .callback = triangular_waveform,
        .name = "MyFctName"
		};
    esp_timer_handle_t esp_timer_handle;
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);

	/* Start timer  */
    esp_timer_start_periodic(esp_timer_handle, 50000);

	/* Display timer information */
	esp_timer_dump(stdout);

	

	/* Re-Start timer  */
    //esp_timer_start_periodic(esp_timer_handle, timeUs);

	xTaskResumeAll();

	// Wait for 15 seconds
    vTaskDelay(TRIANGLE_PERIOD_1 / portTICK_PERIOD_MS);

	/* Stop Timer */
	esp_timer_stop(esp_timer_handle);

	/* Delete Timer */
	esp_timer_delete(esp_timer_handle);

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}

