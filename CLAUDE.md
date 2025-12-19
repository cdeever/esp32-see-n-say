# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESP32-based See-N-Say toy hack that:
1. Monitors the See-N-Say's speaker output via ADC
2. Detects which of 12 sounds is playing based on ADC input analysis
3. Plays a different custom audio sample in response via I2S output
4. Stores audio samples on micro-SD card (to avoid bloating binary size)

**Current Status**: Code is copied from esp32pumpkin project and needs refactoring for ADC-based audio detection and SD card support.

Target hardware: ESP32-S2, FreeRTOS-based.

## Build System

This project uses ESP-IDF (Espressif IoT Development Framework) version 5.3.1 with CMake.

### Essential Commands

```bash
# Build the project
idf.py build

# Flash to ESP32-S2
idf.py flash

# Monitor serial output
idf.py monitor

# Flash and monitor in one command
idf.py flash monitor

# Clean build artifacts
idf.py fullclean

# Set target (already configured for esp32s2)
idf.py set-target esp32s2

# Configure project (menuconfig)
idf.py menuconfig
```

## Architecture

### Planned System Design

The system needs to:
1. **ADC Input**: Sample See-N-Say speaker output (with appropriate resistors/capacitors for level shifting/protection)
2. **Audio Detection**: Analyze ADC samples to identify which of 12 sounds is playing
3. **SD Card Storage**: Read replacement audio samples from micro-SD card
4. **I2S Output**: Stream selected sample to I2S DAC/amplifier
5. **Detection Logic**: Map detected sound → replacement sound file on SD card

### Hardware Configuration

**Target**: ESP32-S2

**Current Pin Assignments** (from pumpkin, defined in `main/seensay.c`):
- I2S_BCK_IO: GPIO 37 (I2S Bit Clock)
- I2S_WS_IO: GPIO 33 (I2S Word Select/LR Clock)
- I2S_DO_IO: GPIO 34 (I2S Data Out)
- PIR_IO: GPIO 38 (Motion Sensor Input - **will be replaced with ADC input**)
- RGB_IO: GPIO 39 (NeoPixel Data - **may not be needed**)

**Planned Additions**:
- ADC GPIO (TBD): See-N-Say speaker output monitoring
- SD Card SPI pins (TBD): MOSI, MISO, CLK, CS

### FreeRTOS Task Architecture (Planned)

The application will need these concurrent tasks:

1. **adc_monitor_task**: Continuously sample ADC input from See-N-Say speaker
2. **audio_detection_task**: Analyze ADC samples to detect which of 12 sounds is playing
3. **sd_card_task**: Handle SD card I/O, read audio files on demand
4. **i2s_playback_task**: Stream audio from SD card to I2S output

**Current Tasks (from pumpkin)**: PIR sensor polling, audio playback, NeoPixel effects - these will be refactored/replaced.

### Audio System

**Current (from pumpkin)**:
- **Format**: 8-bit signed PCM samples, converted to 16-bit for I2S output
- **Sample Rate**: Variable, defined in included audio header file
- **Audio Headers**: PCM audio data included as C header files (e.g., `132806__nanakisan__evil-laugh-12.h`)
- **I2S Configuration**: Master TX mode, 16-bit samples, DMA with 8 buffers of 1024 bytes each

**Planned Changes**:
- Replace embedded audio headers with SD card storage
- Stream audio from SD card files (WAV or raw PCM format)
- Keep I2S configuration for output
- Add ADC sampling for input analysis (need to determine sample rate for detection)

### Dependencies

**Current** (managed via ESP-IDF component manager in `idf_component.yml`):
- `espressif/led_strip` (v2.5.5): NeoPixel/WS2812 LED control (may be removed)

**Planned Additions**:
- SD card driver (built into ESP-IDF via `esp_vfs_fat` and `sdmmc`)
- ADC driver (built into ESP-IDF via `esp_adc`)

## Code Structure Issues

**Note**: The CMakeLists.txt references files that don't exist in the repository:
- `main/esp32pumpkin.c` (CMakeLists references this but actual file is `seensay.c`)
- `main/neopixel.c` (referenced but missing)
- `main/neopixel.h` (included in seensay.c but missing)
- Audio header files (e.g., `132806__nanakisan__evil-laugh-12.h`) are not in the repository

When working with this codebase, you may need to address these missing files or update the build configuration.

## Audio Detection Approach (Planned)

The key technical challenge is detecting which of 12 See-N-Say sounds is currently playing:

1. **ADC Sampling**: Continuously sample speaker output via ADC (need voltage divider/coupling circuit)
2. **Feature Extraction**: Possible approaches:
   - Amplitude pattern matching
   - Frequency analysis (FFT)
   - Template matching against known waveforms
   - Simple threshold/timing detection
3. **Sound Classification**: Map detected pattern to one of 12 triggers
4. **SD Card Lookup**: Load corresponding replacement audio file
5. **I2S Playback**: Stream replacement audio

**Design Decision Needed**: Detection algorithm (simple amplitude vs. FFT vs. pattern matching)

## Development Notes

- Current code is from esp32pumpkin and needs major refactoring for ADC input and SD card support
- The I2S output driver code can be reused but may use deprecated APIs (check ESP-IDF 5.3.1 compatibility)
- Binary size was a problem with embedded audio - SD card solves this
- Will need to add external circuit for ADC input (voltage divider, DC blocking cap, protection)
- ESP32-S2 ADC characteristics may affect detection accuracy
- Stack sizes for tasks may need adjustment based on SD card and FFT requirements
