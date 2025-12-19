#include "i2s_output.h"
#include "config.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include <string.h>

// Module-local event group handle
static EventGroupHandle_t i2s_event_group = NULL;

// Global state for detected sound ID (shared with audio_detection module)
extern volatile int detected_sound_id;

// Initialize I2S hardware
esp_err_t i2s_output_init(void) {
    ESP_LOGI(TAG_I2S, "Initializing I2S...");

    // I2S driver configuration
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,
        .sample_rate = CONFIG_I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = CONFIG_I2S_DMA_BUF_COUNT,
        .dma_buf_len = CONFIG_I2S_DMA_BUF_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = true,
    };

    // I2S pin configuration
    i2s_pin_config_t pin_config = {
        .bck_io_num = CONFIG_GPIO_I2S_BCK,
        .ws_io_num = CONFIG_GPIO_I2S_WS,
        .data_out_num = CONFIG_GPIO_I2S_DO,
        .data_in_num = I2S_PIN_NO_CHANGE
    };

    // Install and configure I2S driver
    esp_err_t ret = i2s_driver_install(CONFIG_I2S_PORT_NUM, &i2s_config, 0, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_I2S, "Failed to install I2S driver: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = i2s_set_pin(CONFIG_I2S_PORT_NUM, &pin_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_I2S, "Failed to set I2S pins: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = i2s_set_clk(CONFIG_I2S_PORT_NUM, CONFIG_I2S_SAMPLE_RATE,
                      I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_I2S, "Failed to set I2S clock: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG_I2S, "I2S initialized successfully");
    return ESP_OK;
}

// I2S playback task (private to this module)
static void i2s_playback_task(void *pvParameter) {
    ESP_LOGI(TAG_I2S, "I2S playback task started");

    while (1) {
        // Wait for sound detection event
        EventBits_t bits = xEventGroupWaitBits(
            i2s_event_group,
            EVENT_SOUND_DETECTED,
            pdTRUE,   // Clear bit after reading
            pdFALSE,  // Wait for any bit
            portMAX_DELAY
        );

        if (bits & EVENT_SOUND_DETECTED) {
            // TODO: Read audio file from SD card based on detected_sound_id
            // TODO: Stream audio samples to I2S
            ESP_LOGI(TAG_I2S, "Playing replacement audio for sound %d", detected_sound_id);

            // Signal playback complete
            xEventGroupSetBits(i2s_event_group, EVENT_PLAYBACK_COMPLETE);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Start I2S playback task
void i2s_output_start_task(EventGroupHandle_t event_group) {
    i2s_event_group = event_group;

    BaseType_t ret = xTaskCreate(
        i2s_playback_task,
        "i2s_playback",
        CONFIG_TASK_STACK_I2S,
        NULL,
        CONFIG_TASK_PRIORITY_I2S,
        NULL
    );

    if (ret != pdPASS) {
        ESP_LOGE(TAG_I2S, "Failed to create I2S playback task");
    } else {
        ESP_LOGI(TAG_I2S, "I2S playback task created");
    }
}
