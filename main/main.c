#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "config.h"

// Module headers
#include "audio/i2s_output.h"
// TODO: Add these when modules are created:
// #include "adc/adc_monitor.h"
// #include "detection/audio_detection.h"
// #include "sd_card/sd_card.h"

// Global event group for inter-task communication
static EventGroupHandle_t xSystemEvents = NULL;

// Global state: which sound was detected (-1 = none, 0-11 = sound ID)
volatile int detected_sound_id = -1;

void app_main() {
    ESP_LOGI(TAG_MAIN, "===================================");
    ESP_LOGI(TAG_MAIN, "ESP32 See-N-Say Audio Replacement");
    ESP_LOGI(TAG_MAIN, "===================================");

    // Create event group for inter-task synchronization
    xSystemEvents = xEventGroupCreate();
    if (xSystemEvents == NULL) {
        ESP_LOGE(TAG_MAIN, "Failed to create event group!");
        return;
    }

    // ========================================================================
    // Phase 1: Initialize hardware subsystems
    // ========================================================================

    ESP_LOGI(TAG_MAIN, "Initializing subsystems...");

    // Initialize SD card
    // TODO: Uncomment when sd_card module is created
    // esp_err_t ret = sd_card_init();
    // if (ret != ESP_OK) {
    //     ESP_LOGE(TAG_MAIN, "SD card initialization failed!");
    //     return;
    // }

    // Initialize I2S audio output
    esp_err_t ret = i2s_output_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_MAIN, "I2S initialization failed!");
        return;
    }

    // Initialize ADC for See-N-Say monitoring
    // TODO: Uncomment when adc_monitor module is created
    // ret = adc_monitor_init();
    // if (ret != ESP_OK) {
    //     ESP_LOGE(TAG_MAIN, "ADC initialization failed!");
    //     return;
    // }

    ESP_LOGI(TAG_MAIN, "All subsystems initialized");

    // ========================================================================
    // Phase 2: Start RTOS tasks
    // ========================================================================

    ESP_LOGI(TAG_MAIN, "Starting tasks...");

    // Start I2S playback task
    i2s_output_start_task(xSystemEvents);

    // Start ADC monitoring task
    // TODO: Uncomment when adc_monitor module is created
    // adc_monitor_start_task(xSystemEvents);

    // Start audio detection task
    // TODO: Uncomment when audio_detection module is created
    // audio_detection_start_task(xSystemEvents);

    ESP_LOGI(TAG_MAIN, "===================================");
    ESP_LOGI(TAG_MAIN, "System ready - waiting for sounds");
    ESP_LOGI(TAG_MAIN, "===================================");

    // Main task can exit - FreeRTOS tasks will continue running
}
