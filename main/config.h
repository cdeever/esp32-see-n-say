#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// GPIO Pin Assignments
// ============================================================================

// I2S Audio Output
#define CONFIG_GPIO_I2S_BCK        (37)  // Bit Clock
#define CONFIG_GPIO_I2S_WS         (33)  // Word Select (LR Clock)
#define CONFIG_GPIO_I2S_DO         (34)  // Data Out

// ADC Input for See-N-Say Detection
// TODO: Determine actual pin/channel for ESP32-S2
#define CONFIG_GPIO_ADC_INPUT      (1)   // GPIO1 = ADC1_CH0 on ESP32-S2
#define CONFIG_ADC_CHANNEL         ADC1_CHANNEL_0
#define CONFIG_ADC_ATTEN           ADC_ATTEN_DB_11  // Full range 0-3.3V
#define CONFIG_ADC_WIDTH           ADC_WIDTH_BIT_12

// SD Card SPI Pins (ESP32-S2)
// TODO: Verify pins for your specific board
#define CONFIG_GPIO_SD_MOSI        (11)
#define CONFIG_GPIO_SD_MISO        (13)
#define CONFIG_GPIO_SD_CLK         (12)
#define CONFIG_GPIO_SD_CS          (10)

// ============================================================================
// I2S Configuration
// ============================================================================
#define CONFIG_I2S_PORT_NUM        (0)
#define CONFIG_I2S_SAMPLE_RATE     (44100)
#define CONFIG_I2S_DMA_BUF_COUNT   (8)
#define CONFIG_I2S_DMA_BUF_LEN     (1024)

// ============================================================================
// Audio Detection Configuration
// ============================================================================
#define CONFIG_NUM_SOUNDS          (12)    // 12 different See-N-Say sounds
#define CONFIG_ADC_SAMPLE_RATE     (8000)  // 8kHz for detection (TODO: tune)

// ============================================================================
// SD Card Configuration
// ============================================================================
#define CONFIG_SD_MOUNT_POINT      "/sdcard"
#define CONFIG_SD_MAX_FILES        (5)

// ============================================================================
// Task Stack Sizes (bytes)
// ============================================================================
#define CONFIG_TASK_STACK_I2S      (4096)
#define CONFIG_TASK_STACK_ADC      (4096)
#define CONFIG_TASK_STACK_DETECT   (4096)

// ============================================================================
// Task Priorities
// ============================================================================
#define CONFIG_TASK_PRIORITY_ADC   (6)     // Highest - time-sensitive ADC sampling
#define CONFIG_TASK_PRIORITY_DETECT (5)    // High - process ADC data
#define CONFIG_TASK_PRIORITY_I2S   (5)     // High - audio playback

// ============================================================================
// Event Bits for FreeRTOS Event Groups
// ============================================================================
#define EVENT_SOUND_DETECTED       (1 << 0)  // Audio detection found a sound
#define EVENT_PLAYBACK_COMPLETE    (1 << 1)  // I2S playback finished

// ============================================================================
// Logging Tags
// ============================================================================
#define TAG_MAIN         "MAIN"
#define TAG_I2S          "I2S"
#define TAG_ADC          "ADC"
#define TAG_DETECTION    "DETECT"
#define TAG_SD_CARD      "SD_CARD"

#endif // CONFIG_H
