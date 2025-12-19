#ifndef I2S_OUTPUT_H
#define I2S_OUTPUT_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_err.h"

// Initialize I2S hardware
esp_err_t i2s_output_init(void);

// Start I2S playback task
void i2s_output_start_task(EventGroupHandle_t event_group);

#endif // I2S_OUTPUT_H
