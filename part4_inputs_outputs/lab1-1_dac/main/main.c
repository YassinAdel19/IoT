#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_timer.h"
#include "driver/dac.h"

static const uint32_t STACK_SIZE = 4000;
static const uint32_t T2_PRIO = 6;

#define DAC_CHANNEL DAC_CHANNEL_1
#define TRIANGLE_PERIOD_1 5000000  // 5 seconds in microseconds
#define TRIANGLE_PERIOD_2 500000   // 0.5 seconds in microseconds

SemaphoreHandle_t waveformStopSemaphore;

void triangular_waveform_task(void *pvParameters) {
    uint32_t amplitude = 0;
    uint32_t increment = 1;

    while (1) {
        if (xSemaphoreTake(waveformStopSemaphore, 0) == pdTRUE) {
            break;
        }

        dac_output_voltage(DAC_CHANNEL, amplitude);

        vTaskDelay(pdMS_TO_TICKS(1));

        amplitude += increment;

        if (amplitude == 255 || amplitude == 0) {
            increment = -increment;
        }
    }

    vSemaphoreDelete(waveformStopSemaphore);
    vTaskDelete(NULL);
}

void app_main(void) {
    // Configure DAC
    dac_output_enable(DAC_CHANNEL);

    // Create semaphore for stopping the waveform task
    waveformStopSemaphore = xSemaphoreCreateBinary();

    // Create and start the triangular waveform task with a period of 5 seconds
    xTaskCreate(triangular_waveform_task, "triangular_waveform_task", STACK_SIZE, NULL, T2_PRIO, NULL);
    vTaskDelay(TRIANGLE_PERIOD_1 / portTICK_PERIOD_MS);

    // Change the waveform period to 0.5 seconds
    vTaskSuspendAll();
    esp_timer_handle_t esp_timer_handle;
    esp_timer_create_args_t esp_timer_create_args = {
        .callback = NULL,  // Replace with your callback function if needed
        .name = "TriangularWaveformTimer"
    };
    esp_timer_create(&esp_timer_create_args, &esp_timer_handle);
    esp_timer_start_periodic(esp_timer_handle, TRIANGLE_PERIOD_2);
    xTaskResumeAll();

    // Wait for 15 seconds
    vTaskDelay(TRIANGLE_PERIOD_1 / portTICK_PERIOD_MS);

    // Stop the waveform
    xSemaphoreGive(waveformStopSemaphore);

    // Delete Timer
    esp_timer_stop(esp_timer_handle);
    esp_timer_delete(esp_timer_handle);

    // Ensure clean exit
    vTaskDelete(NULL);
}

